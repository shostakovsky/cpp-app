# Каркас проекта: спецификация

Что должно быть в новом проекте C++ до первой строки предметного кода.
Документ — техническое задание для агента (скилл `new-cpp-project`) и
контрольный список для человека. Критерий приёмки один: каталог проходит
`python checks/scaffold.py <каталог>` из репозитория курса.

Корневой CMake-проект всегда называется `MyEpicProject`.
Имя приложения (подпроект, цели, каталог `src/…`) задаётся при генерации;
ниже оно обозначено `<Name>` (пример: `MyEpicApp`).
Спецификация самодостаточна: всё, что нужно для генерации, записано здесь;
чего здесь нет, того в сгенерированном проекте быть не должно.

## 1. Структура

```text
<name>/
├── CMakeLists.txt                 # корень: project(MyEpicProject), C++23, модули, add_subdirectory
├── conanfile.txt                  # nlohmann_json/3.12.0, gtest/1.18.0; generators CMakeDeps, CMakeToolchain
├── config.json                    # {"title": "<Name>", "window": {"width": 800, "height": 600}}
├── README.md                      # по схеме из раздела 9
├── .gitignore                     # build/, build-asan/, .vs/, CMakeUserPresets.json, __pycache__/, docs/api/, LastCoverageResults.log
├── .clang-format                  # Microsoft base, Allman, 4 пробела, ColumnLimit 140
├── .clang-tidy                    # см. раздел 6
├── Doxyfile                       # см. раздел 8
├── mkdocs.yml, requirements-docs.txt, docs/{index,architecture,development}.md
├── protection.json                # см. раздел 10
├── profiles/
│   ├── windows-msvc-debug, windows-msvc-release    # msvc, cppstd=23, runtime dynamic, version через detect_api
│   └── linux-gcc-debug, linux-gcc-release          # gcc, libstdc++11, cppstd=23; version через detect_api; executables gcc/g++
├── cmake/
│   ├── CompilerWarnings.cmake     # set_project_warnings(target): /W4 /WX /permissive- | -Wall -Wextra -Wpedantic -Werror
│   ├── Sanitizers.cmake           # option(ENABLE_ASAN), enable_sanitizers(target), см. раздел 7
│   └── ClangFormat.cmake          # цели format и format-check, поиск clang-format в PATH и в Visual Studio
├── automation/                    # см. раздел 4
├── src/<Name>/
│   ├── CMakeLists.txt             # project(<Name>); цели <Name>Lib STATIC, <Name>, <Name>TestRunner
│   ├── Config.h, Config.cpp       # LifeExe::AppConfig, LifeExe::parseConfig(std::istream&)
│   ├── main.cpp                   # читает config.json, печатает приветствие, исключения не выходят из main
│   └── Tests/ConfigTest.cpp       # GoogleTest, четыре теста: ShouldReadTitleAndWindow, ShouldNameMissingKey,
│                                  #   ShouldRejectWrongValueType, ShouldRejectInvalidJson
└── .github/workflows/
    ├── build.yml                  # матрица Debug/Release на Windows и Linux, артефакты сборки
    ├── tests.yml                  # Windows: ASan, тесты, покрытие --min 70, отчёты; Linux: ASan+UBSan, тесты
    ├── code-quality.yml           # clang-format --check, clang-tidy на Windows (MSBuild) и Linux (compile_commands)
    └── docs.yml                   # Doxygen + MkDocs, публикация на Pages только с основной ветки
```

## 2. Сборка

- CMake 3.25+, `CMAKE_CXX_STANDARD 23`, `CMAKE_CXX_STANDARD_REQUIRED ON`.
- Корень: `project(MyEpicProject LANGUAGES CXX)`, `find_package(nlohmann_json REQUIRED)`,
  `find_package(GTest REQUIRED)`, `enable_testing()`, `USE_FOLDERS ON`,
  `CMAKE_RUNTIME_OUTPUT_DIRECTORY` и `CMAKE_ARCHIVE_OUTPUT_DIRECTORY` =
  `${CMAKE_BINARY_DIR}/bin`, `VS_STARTUP_PROJECT <Name>`,
  `include(cmake/CompilerWarnings.cmake)`, `include(cmake/Sanitizers.cmake)` **до**
  `add_subdirectory(src/<Name>)`, `include(cmake/ClangFormat.cmake)` после.
- Подпроект: имена целей только через `${PROJECT_NAME}`, `${PROJECT_NAME}Lib`,
  `${PROJECT_NAME}TestRunner`. Библиотека линкует `nlohmann_json::nlohmann_json`
  `PRIVATE`, `target_include_directories(... PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})`.
  Исполняемый файл — только `main.cpp`. Тест-раннер линкует библиотеку и
  `GTest::gtest_main`; `gtest_discover_tests(... DISCOVERY_TIMEOUT 30)`.
  `set_project_warnings` и `enable_sanitizers` на всех трёх целях.
  `VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}` у исполняемого файла.
- Conan 2: `conan install` в `build/` (Windows, обе конфигурации в один каталог)
  или `build/<Config>/` (Linux), `--build=missing`, toolchain через
  `-DCMAKE_TOOLCHAIN_FILE`.
- Профили Windows: `detect_api.detect_default_compiler()`,
  `compiler={{ compiler }}`, `compiler.version={{ version }}`,
  `compiler.runtime=dynamic`, `compiler.runtime_type` = Debug|Release,
  `compiler.cppstd=23`, `os=Windows`.
- Профили Linux: `detect_api` + `default_compiler_version`, `compiler=gcc`,
  `compiler.version={{ version }}`, `compiler.libcxx=libstdc++11`,
  `compiler.cppstd=23`, `os=Linux`;
  `tools.build:compiler_executables={"c": "gcc", "cpp": "g++"}`.
  В CI на Ubuntu 24.04 перед сборкой: установить `g++-14` и сделать его
  системным `gcc`/`g++` через `update-alternatives` (профили вызывают `gcc`/`g++`).

## 3. Код

- Пространство имён `LifeExe`. Типы PascalCase, функции camelCase, поля с
  brace-init (`int width{0}`), `const` по умолчанию, параметры-строки как
  `std::string_view`, форматирование через `std::format`/`std::println`.
- `AppConfig`: поля `title` (`std::string`), `width`/`height` (`int`, brace-init 0).
- `parseConfig` читает `std::istream`, явные `.get<T>()`; отсутствующий ключ,
  неверный тип или невалидный JSON — `std::runtime_error` с именем ключа /
  текстом ошибки разбора.
- `main`: открывает `config.json`, вызывает `parseConfig`, печатает
  `Hello, {title}! Window: {width}x{height}` через `std::println`; всё тело в
  `try`, `catch (const std::exception&)` и `catch (...)`, в обработчиках только
  `std::fputs`; `EXIT_SUCCESS`/`EXIT_FAILURE`.
- Тесты (`ConfigTest`): `ShouldReadTitleAndWindow`, `ShouldNameMissingKey`,
  `ShouldRejectWrongValueType`, `ShouldRejectInvalidJson`.
- Публичный заголовок документирован `///` (namespace, struct, поля `///<`,
  функция с `@param`, `@return`, `@throws`).

## 4. Скрипты `automation/` (Python 3, без сторонних пакетов)

`common.py` — единственное место, где различаются платформы: `IS_WINDOWS`,
`profile(configuration)`, `build_dir(configuration, root="build")`,
`bin_dir(configuration)`, `configure(...)` (на Linux `-DCMAKE_BUILD_TYPE` и
`-DCMAKE_EXPORT_COMPILE_COMMANDS=ON`), `ensure_generated(...)`,
`cmake_build(...)` (`--config` только на Windows), `run(...)` с выводом
команды и остановкой при ошибке.

| Скрипт | Делает |
|---|---|
| `generate_project_files.py` | conan install для обеих конфигураций, конфигурация CMake |
| `build_debug.py`, `build_release.py` | сборка конфигурации |
| `run_tests.py` | сборка Debug, `ctest --output-on-failure --output-junit test-report.xml` |
| `run_coverage.py` | Windows: OpenCppCoverage по `src/<Name>`, HTML + Cobertura, таблица, `GITHUB_STEP_SUMMARY`, `--min N`; Linux: сообщение и выход 0 |
| `run_tidy.py` | Windows: `cmake --build build --config Debug -- /p:RunCodeAnalysis=true /p:EnableClangTidyCodeAnalysis=true /p:EnableMicrosoftCodeAnalysis=false` — свойства уходят MSBuild через CMake, `MSBuild.exe` напрямую не вызывается; Linux: `clang-tidy -p build/Debug` по всем `.cpp` из `src/` |
| `run_asan.py` | отдельный каталог `build-asan` через `build_dir(..., root="build-asan")`, `-DENABLE_ASAN=ON`, ctest |
| `format_files.py` | цель `format`; `--check` — `format-check` |
| `build_docs.py` | `doxygen Doxyfile`, затем `mkdocs build --strict`; `--serve` |
| `clean.py` | `git clean -Xdn` / `-Xdf`; `--dry-run` |

## 5. Форматирование

`.clang-format` целиком:

```yaml
BasedOnStyle: Microsoft
IndentWidth: 4
UseTab: Never
TabWidth: 4
BreakBeforeBraces: Allman
ColumnLimit: 140
AccessModifierOffset: -4
SortIncludes: false
AllowShortBlocksOnASingleLine: false
AlignAfterOpenBracket: DontAlign
AllowShortFunctionsOnASingleLine: Inline
PointerAlignment: Left
AllowShortIfStatementsOnASingleLine: true
SpacesBeforeTrailingComments: 2
AllowShortCaseLabelsOnASingleLine: true
IndentCaseLabels: true
AlwaysBreakTemplateDeclarations: Yes
```

`ClangFormat.cmake` ищет `clang-format` в PATH и в каталогах Visual Studio
(`VC/Tools/Llvm/bin`, `VC/Tools/Llvm/x64/bin`); цели `format` и
`format-check` (`--dry-run --Werror`) по `src/**/*.cpp`, `src/**/*.h`.

## 6. Статический анализ

`.clang-tidy`: `Checks: '-*,bugprone-*,performance-*,modernize-*,
-modernize-use-trailing-return-type,readability-*,-readability-magic-numbers,
-readability-identifier-length,misc-const-correctness'`,
`WarningsAsErrors: '*'`, `HeaderFilterRegex: 'src[/\\]<Name>[/\\].*'`,
`FormatStyle: file`.

## 7. Санитайзеры

`Sanitizers.cmake`: `option(ENABLE_ASAN "Build with AddressSanitizer" OFF)`.
MSVC: вырезать `/RTC1` из `CMAKE_CXX_FLAGS_DEBUG`; найти
`clang_rt.asan_dynamic-x86_64.dll` рядом с компилятором (`FATAL_ERROR`, если
нет); `/fsanitize=address`, `_DISABLE_STL_ANNOTATION`, `/INCREMENTAL:NO`,
`POST_BUILD` копирование DLL в `$<TARGET_FILE_DIR>` для исполняемых целей.
GCC/Clang: `-fsanitize=address,undefined -fno-sanitize-recover=all
-fno-omit-frame-pointer` при компиляции и `-fsanitize=address,undefined`
при линковке.

## 8. Документация

`Doxyfile` только с отличиями от умолчаний: `PROJECT_NAME = <Name>`,
`PROJECT_BRIEF = "Application logic library and executable"`,
`OUTPUT_DIRECTORY = docs`, `HTML_OUTPUT = api`, `INPUT = src/<Name>`,
`FILE_PATTERNS = *.h`, `RECURSIVE = NO`, `EXCLUDE = src/<Name>/Tests`,
`EXTRACT_ALL = NO`, `WARN_IF_UNDOCUMENTED = YES`,
`WARN_AS_ERROR = FAIL_ON_WARNINGS`, `QUIET = YES`, `GENERATE_LATEX = NO`,
`HAVE_DOT = NO`, `JAVADOC_AUTOBRIEF = YES`.
`requirements-docs.txt`: `mkdocs==1.6.1`, `mkdocs-material==9.7.7`.
`mkdocs.yml`: `site_name: <Name>`, `docs_dir: docs`, `site_dir: build/docs/site`,
тема `material` с `language: ru`, `nav` из трёх страниц и `api/index.html`,
расширения `admonition`, `pymdownx.superfences`. Страницы по 30–60 строк.

## 9. README

Обязательный порядок разделов (как в каркасе). Команды в тексте совпадают со
скриптами `automation/`. Адреса значков CI и сайта Pages строятся от
`<owner>/<repo>` на GitHub; до создания репозитория — заполнители `<owner>` и
`<repo>`.

1. **Заголовок** — `# <Name>`; сразу под ним четыре значка workflow:
   `build.yml`, `tests.yml`, `code-quality.yml`, `docs.yml`.
2. **Краткое описание** — два-три предложения: что за проект и какой стек
   (C++23, CMake, Conan 2, MSVC / GCC, `automation/`).
3. **`## Системные требования`**
   - `### Windows` — VS 2022, CMake 3.25+, Python 3, Conan 2, Doxygen /
     `requirements-docs.txt`; путь к бинарникам `build/bin/<Config>/`.
   - `### Linux` — для Ubuntu 24.04: `g++-14`, cmake, make, clang-tidy,
     clang-format, pipx/conan; путь к бинарникам `build/<Config>/bin/`.
4. **`## Сборка и запуск`** — из корня проекта:
   `generate_project_files.py`, `build_debug.py`, запуск
   `build/bin/Debug/<Name>.exe` (Windows); отдельно упомянуть
   `build_release.py` и `clean.py`.
5. **`## Тестирование`** — `run_tests.py`, `run_coverage.py` (в т.ч. `--min 70`
   в CI), `run_asan.py`.
6. **`## Контроль качества кода`** — `format_files.py --check`, `run_tidy.py`;
   ссылка на `.clang-format` и `.clang-tidy`; упоминание
   `format_files.py` без `--check`.
7. **`## Документация`** — URL GitHub Pages; `build_docs.py` и
   `build_docs.py --serve`.
8. **`## Структура проекта`** — дерево каталогов (`src/<Name>/`, `automation/`,
   `profiles/`, `cmake/`, `.github/workflows/`, корневые файлы) и краткие
   пояснения.
9. **`## Непрерывная интеграция (CI)`** — четыре workflow; таблица
   (файл / задачи / артефакты); правило защиты ветки и девять contexts из
   `protection.json`.

Все скрипты из раздела 4 (`generate_project_files.py`, `build_debug.py`,
`build_release.py`, `run_tests.py`, `run_coverage.py`, `run_asan.py`,
`format_files.py`, `run_tidy.py`, `build_docs.py`, `clean.py`) упомянуты в
README хотя бы один раз.

## 10. CI

Четыре файла: `build.yml`, `tests.yml`, `code-quality.yml`, `docs.yml`.

### Общие правила для всех workflow

- триггеры: `on: push: branches: [master]` и `pull_request`;
- `concurrency.group`: `${{ github.workflow }}-${{ github.ref }}`;
  `cancel-in-progress: true` (у `docs.yml` — `false`);
- раннеры: `windows-latest` и при необходимости `ubuntu-latest`;
- Linux-джобы: установить `g++-14`, через `update-alternatives` сделать
  системными `gcc`/`g++`, Conan через `pipx install conan`; на Windows —
  `pip install conan`;
- кэш Conan: `actions/cache@v6`, путь `~/.conan2/p`, ключ
  `conan-${{ runner.os }}-${{ hashFiles('conanfile.txt', 'profiles/**') }}`;
- версии actions для **нового** каркаса (как в актуальных спринтах курса):
  `checkout@v7`, `upload-artifact@v7`, `upload-pages-artifact@v3`,
  `deploy-pages@v4`, `phoenix-actions/test-reporting@v10`.
  Уже существующий учебный репозиторий может оставаться на прежних major
  (`@v4` и т.п.) до отдельного обновления.

### Workflow по файлам

1. **`build.yml`**
   - Windows, матрица Debug/Release → имена работ `build-Debug`,
     `build-Release`; скрипты `generate_project_files.py`,
     `build_debug.py` / `build_release.py`; артефакты `app-Debug`,
     `app-Release` (`build/bin/<Config>/<Name>.exe` + `config.json`).
   - Linux, матрица Debug/Release → имена `linux-Debug`, `linux-Release`;
     те же скрипты через `python3`; артефакты `app-linux-Debug`,
     `app-linux-Release` (`build/<Config>/bin/<Name>` + `config.json`).
2. **`tests.yml`**
   - Windows, работа `tests`: `run_asan.py`, `run_tests.py`,
     `run_coverage.py --min 70`; артефакты `coverage-report`,
     `test-report`; публикация JUnit через `test-reporting@v10`.
   - Linux, работа `linux-tests`: `run_asan.py`, `run_tests.py`.
3. **`code-quality.yml`**
   - Windows, работа `code-quality`: `format_files.py --check`,
     `run_tidy.py`.
   - Linux, работа `linux-tidy`: `run_tidy.py` (в установку — ещё
     `clang-tidy`).
4. **`docs.yml`**
   - Windows, работа `docs`: Doxygen (choco), MkDocs из
     `requirements-docs.txt`, `build_docs.py`; артефакт Pages из
     `build/docs/site`.
   - работа `deploy`: `needs: docs`, `environment: github-pages`, только
     `push` в основную ветку; `actions/deploy-pages@v4`.

### Защита ветки (`protection.json`)

`required_status_checks.contexts` — ровно девять имён:

`build-Debug`, `build-Release`, `linux-Debug`, `linux-Release`, `tests`,
`linux-tests`, `code-quality`, `linux-tidy`, `docs`.

`strict: false`, `enforce_admins: true`, ревью не требуется
(`required_pull_request_reviews: null`).

## 11. Приёмка

```powershell
python <курс>/checks/scaffold.py <каталог проекта> --report reports/scaffold-report.txt
```

Все проверки спринтов cpp-engineering 04–14 в локальной части зелёные. Что не
проходит — правится в этой спецификации, не в сгенерированных файлах.

## Спорные / непоследовательные места в исходном проекте

1. **Имена CMake:** в учебном репозитории корень — `CppCourse`, приложение —
   `App`. В каркасе для генерации: корень всегда `MyEpicProject`, приложение —
   параметр `<Name>` (пример имени — `MyEpicApp`).
2. **`run_coverage.py` в учебном `project/`** содержит литералы `App` и
   `AppTestRunner.exe`. В **сгенерированном** каркасе пути и имя раннера
   строить от `<Name>` (`src/<Name>`, `<Name>TestRunner.exe`), не копировать
   строку `App` из эталона.
