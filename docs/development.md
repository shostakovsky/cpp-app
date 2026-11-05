# Разработка

Локальная сборка, тесты и проверки качества запускаются теми же скриптами, что
и CI — одни и те же файлы `automation/` работают и на Windows, и на Linux
(`python` / `python3`). Общие пути и вызов subprocess собраны в
`automation/common.py`: скрипты можно вызывать из любого текущего каталога.

Единственный источник настроек сборки — `CMakePresets.json`. Скрипты, CI и
IDE используют одни и те же пресеты `<платформа>-<вариант>`; у каждого свой
каталог `build/<пресет>/`, бинарники — в `build/<пресет>/bin/`. Генератор
везде Ninja, одна конфигурация на каталог:

| Пресет | Назначение |
| --- | --- |
| `windows-msvc-debug`, `linux-gcc-debug` | Debug: сборка, тесты, анализ |
| `windows-msvc-release`, `linux-gcc-release` | Release |
| `windows-msvc-asan`, `linux-gcc-asan` | Debug с ASan (на Linux ещё UBSan) |

Для каждого пресета есть workflow-пресет с тем же именем:
`cmake --workflow --preset linux-gcc-debug` конфигурирует, собирает и
запускает тесты одной командой.

Ninja на Windows нужно окружение MSVC (`cl.exe`, Windows SDK). Если скрипт
запущен не из Developer Command Prompt, `common.py` находит Visual Studio
через `vswhere` и подгружает окружение из `vcvars64.bat`.

## Скрипты `automation/`

| Скрипт | Назначение |
| --- | --- |
| `generate_project_files.py` | Конфигурация пресетов Debug и Release (Conan — внутри) |
| `build_debug.py` | Сборка конфигурации Debug |
| `build_release.py` | Сборка конфигурации Release |
| `clean.py` | Удаление сборочных каталогов (`--dry-run` — только список) |
| `format_files.py` | Форматирование; `--check` — только проверка стиля |
| `run_tests.py` | Debug + CTest; JUnit в `build/<пресет>/test-report.xml` |
| `run_coverage.py` | Покрытие OpenCppCoverage; опционально `--min N` |
| `run_asan.py` | Workflow-пресет `*-asan`: сборка с ASan и прогон тестов |
| `run_tidy.py` | clang-tidy по `compile_commands.json` (обе платформы) |
| `build_docs.py` | Документация проекта. Doxygen + MkDocs |

Конфигурации стиля: `.clang-format`, `.clang-tidy`. Профили Conan:
`profiles/windows-msvc-{debug,release}`, `profiles/linux-gcc-{debug,release}`.

## Работа в IDE

Visual Studio, CLion и VS Code открывают проект как CMake-папку по
`CMakePresets.json`, без предварительного запуска скриптов, и собирают в те же
каталоги `build/<пресет>/`, что и скрипты:

- Visual Studio: File → Open → Folder, конфигурация `windows-msvc-debug`;
- CLion: Settings → Build, Execution, Deployment → CMake, включить пресеты `linux-gcc-*`.

`config.json` копируется в `build/<пресет>/bin/` рядом с `App`, поэтому
приложение запускается и отлаживается из IDE без настройки рабочего каталога.

Пресет подключает `cmake/conan_provider.cmake` (dependency provider из
[cmake-conan](https://github.com/conan-io/cmake-conan)): на первом
`find_package()` CMake сам вызывает `conan install` с профилем из `profiles/`
и ставит зависимости в `build/<пресет>/conan`. Удаление каталога сборки
(«Delete Cache» в Visual Studio) безопасно: следующий configure восстановит всё сам.

`conan_provider.cmake` скопирован без изменений с коммита
[`dff3768`](https://github.com/conan-io/cmake-conan/blob/dff3768b8756f8982be4bf987da72ed5bf4b9981/conan_provider.cmake)
ветки `develop2` — последнего, работающего с генератором `CMakeDeps`. Обновление —
вручную: заменить файл версией с нового коммита и проверить пресеты.

## Перед коммитом

Минимальный чек-лист (из корня `project/`):

1. `python automation/format_files.py` — привести стиль, затем

   `python automation/format_files.py --check`.
2. `python automation/build_debug.py` и `python automation/run_tests.py`.
3. При изменениях логики — `python automation/run_coverage.py` (в CI порог 70%).
4. При работе с памятью/указателями — `python automation/run_asan.py`.
5. Перед push в основную ветку — `python automation/run_tidy.py`.

На GitHub Actions те же шаги выполняются на `windows-latest` и `ubuntu-latest`
при каждом `push` в `master` и pull request (см. `.github/workflows/`).

## Сборка документации

Контур Docs-as-Code:

1. **Doxygen** (`Doxyfile`) читает публичные заголовки (`src/App/*.h`) и

   пишет HTML-справочник в `docs/api/` (каталог в `.gitignore`).
2. **MkDocs** (`mkdocs.yml`, тема Material) собирает Markdown из `docs/` и

   копирует `docs/api` в итоговый сайт `build/docs/site`.

Зависимости портала: `pip install -r requirements-docs.txt`.
Локальный просмотр: `python -m mkdocs serve`.
Строгая сборка: `python -m mkdocs build --strict` (битая ссылка — ошибка).

Перед MkDocs всегда нужен актуальный вывод Doxygen: иначе пункт навигации
«Справочник API» (`api/index.html`) сломает `--strict`.
