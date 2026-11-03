# App

[![build](https://github.com/shostakovsky/cpp-app/actions/workflows/build.yml/badge.svg)](https://github.com/shostakovsky/cpp-app/actions/workflows/build.yml)
[![tests](https://github.com/shostakovsky/cpp-app/actions/workflows/tests.yml/badge.svg)](https://github.com/shostakovsky/cpp-app/actions/workflows/tests.yml)
[![code-quality](https://github.com/shostakovsky/cpp-app/actions/workflows/code-quality.yml/badge.svg)](https://github.com/shostakovsky/cpp-app/actions/workflows/code-quality.yml)
[![docs](https://github.com/shostakovsky/cpp-app/actions/workflows/docs.yml/badge.svg)](https://github.com/shostakovsky/cpp-app/actions/workflows/docs.yml)

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
├── .github/workflows/ # GitHub Actions (build, tests, code-quality, docs)
├── CMakeLists.txt
├── conanfile.txt
└── config.json        # конфигурация приложения (рабочий каталог запуска)
```

- `src/` — библиотека `AppLib`, исполняемый файл `App` и юнит-тесты.
- `automation/` — единая точка входа для локальной сборки и тех же шагов в CI.
- `profiles/` — настройки компилятора и зависимостей Conan для Windows/MSVC.
- `.github/` — четыре независимых workflow GitHub Actions.

## Непрерывная интеграция (CI)

Четыре параллельных workflow на `push` в `master` и на pull request (пакеты Conan кэшируются по `conanfile.txt` и `profiles/`):

| Workflow | Файл | Задачи | Артефакты |
| --- | --- | --- | --- |
| **build** | `build.yml` | Матрица Debug/Release: Conan, генерация, сборка | `app-Debug`, `app-Release` (`App.exe` + `config.json`) |
| **tests** | `tests.yml` | ASan, юнит-тесты, покрытие (`--min 70`) | `coverage-report` (HTML), `test-report` (JUnit) |
| **code-quality** | `code-quality.yml` | `format_files.py --check`, `run_tidy.py` | — |
| **docs** | `docs.yml` | Doxygen + MkDocs; деплой на GitHub Pages из `master` | сайт [shostakovsky.github.io/cpp-app](https://shostakovsky.github.io/cpp-app/) |
