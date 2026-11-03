# App

[![build](https://github.com/shostakovsky/cpp-app/actions/workflows/build.yml/badge.svg)](https://github.com/shostakovsky/cpp-app/actions/workflows/build.yml)

Консольное приложение App читает параметры окна из `config.json` и выводит их в терминал. Учебный проект курса по инженерному C++: на нём отрабатываются сборка, тесты, качество кода и CI. Стек — C++23, CMake, Conan 2, MSVC (Visual Studio 2022), Python-скрипты в `automation/`.

## Системные требования

- Windows
- Visual Studio 2022 (MSVC, C++ desktop development)
- CMake 3.25+
- Python 3
- Conan 2
- для сборки документации: Doxygen и зависимости из `requirements-docs.txt` (добавятся в этом спринте)

## Сборка и запуск

Все команды — из каталога `project/`. Общие пути и вызовы инструментов заданы в `automation/common.py`.

```powershell
python automation/generate_project_files.py
python automation/build_debug.py
.\build\bin\Debug\App.exe
```

Конфигурация Release: `python automation/build_release.py`. Полная очистка сборочных каталогов: `python automation/clean.py`.

## Тестирование

```powershell
python automation/run_tests.py
```

Собирает Debug (при необходимости) и запускает юнит-тесты через CTest; JUnit-отчёт пишется в `build/test-report.xml`.

```powershell
python automation/run_coverage.py
```

Измеряет покрытие через OpenCppCoverage. HTML-отчёт: `build/coverage/html/index.html`. В CI порог: `python automation/run_coverage.py --min 70`.

```powershell
python automation/run_asan.py
```

Отдельная сборка в `build-asan/` с AddressSanitizer и прогон тестов под ней; обычный `build/` не затрагивается.

## Контроль качества кода

```powershell
python automation/format_files.py --check
python automation/run_tidy.py
```

Стиль кода — `.clang-format` (база Microsoft); правила clang-tidy — `.clang-tidy`. Автоформатирование: `python automation/format_files.py`.

## Документация

Портал: [https://shostakovsky.github.io/cpp-app/](https://shostakovsky.github.io/cpp-app/).

Локальная сборка сайта (Doxygen + MkDocs):

```powershell
python automation/build_docs.py
python automation/build_docs.py --serve
```

Справочник API строится из комментариев `///` в публичных заголовках; архитектурные и рабочие руководства — из Markdown в `docs/`.

## Структура проекта

```text
project/
├── src/App/           # исходники: AppLib, App, тесты
├── automation/        # скрипты генерации, сборки, тестов и проверок
├── profiles/          # профили Conan (Debug / Release)
├── cmake/             # общие модули CMake (warnings, sanitizers, format)
├── .github/workflows/ # GitHub Actions (build.yml)
├── CMakeLists.txt
├── conanfile.txt
└── config.json        # конфигурация приложения (рабочий каталог запуска)
```

- `src/` — библиотека `AppLib`, исполняемый файл `App` и юнит-тесты.
- `automation/` — единая точка входа для локальной сборки и тех же шагов в CI.
- `profiles/` — настройки компилятора и зависимостей Conan для Windows/MSVC.
- `.github/` — workflow непрерывной интеграции на `windows-latest`.

## Непрерывная интеграция (CI)

Пайплайн `.github/workflows/build.yml` на `windows-latest` при каждом `push` и pull request:

1. `generate_project_files.py` — зависимости и генерация сборочного дерева;
2. `build_debug.py` — Debug;
3. `run_asan.py` — тесты под AddressSanitizer;
4. `run_tests.py` — обычные юнит-тесты;
5. `run_coverage.py --min 70` — покрытие с порогом 70%;
6. `build_release.py` — Release;
7. `format_files.py --check` — проверка стиля;
8. `run_tidy.py` — статический анализ.

Артефакты: `app-release` (`build/bin/Release/App.exe` и `config.json`), `coverage-report` (HTML), `test-report` (JUnit XML).
