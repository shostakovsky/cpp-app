# App

[![build](https://github.com/shostakovsky/cpp-app/actions/workflows/build.yml/badge.svg)](https://github.com/shostakovsky/cpp-app/actions/workflows/build.yml)
[![tests](https://github.com/shostakovsky/cpp-app/actions/workflows/tests.yml/badge.svg)](https://github.com/shostakovsky/cpp-app/actions/workflows/tests.yml)
[![code-quality](https://github.com/shostakovsky/cpp-app/actions/workflows/code-quality.yml/badge.svg)](https://github.com/shostakovsky/cpp-app/actions/workflows/code-quality.yml)
[![docs](https://github.com/shostakovsky/cpp-app/actions/workflows/docs.yml/badge.svg)](https://github.com/shostakovsky/cpp-app/actions/workflows/docs.yml)

Консольное приложение App читает параметры окна из `config.json` и выводит их в терминал. Учебный проект курса по инженерному C++: на нём отрабатываются сборка, тесты, качество кода и CI. Стек — C++23, CMake, Conan 2, MSVC (Visual Studio 2022), Python-скрипты в `automation/`.

## Системные требования

### Windows

- Visual Studio 2022 (MSVC, C++ desktop development)
- CMake 3.25+
- Python 3
- Conan 2
- для документации: Doxygen и `pip install -r requirements-docs.txt`

Бинарники: `build/bin/<Config>/` (например `build\bin\Debug\App.exe`).

### Linux

Ubuntu 24.04 (WSL / CI) — пакеты из шага настройки окружения:

```bash
sudo apt install -y g++-14 cmake make clang-tidy clang-format pipx
pipx install conan
```

На Fedora (локальная разработка): GCC ≥ 14 (`gcc-c++`), `cmake`, `make`, `clang-tools-extra` (`clang-tidy`, `clang-format`), `python3`, Conan 2 (`pip` / `pipx`).

Интерпретатор — `python3`. Бинарники: `build/<Config>/bin/` (например `build/Debug/bin/App`).

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
- `profiles/` — профили Conan для Windows/MSVC и Linux/GCC 14.
- `.github/` — четыре независимых workflow GitHub Actions (Windows + Linux).

## Непрерывная интеграция (CI)

Четыре параллельных workflow на `push` в `master` и на pull request (пакеты Conan кэшируются по `conanfile.txt` и `profiles/`). Слияние в `master` возможно только через Pull Request при зелёных обязательных проверках — все девять contexts из `protection.json`: `build-Debug`, `build-Release`, `linux-Debug`, `linux-Release`, `tests`, `linux-tests`, `code-quality`, `linux-tidy`, `docs`.

| Workflow | Файл | Задачи | Артефакты |
| --- | --- | --- | --- |
| **build** | `build.yml` | Windows: матрица Debug/Release. Linux: `linux-Debug`, `linux-Release` (GCC 14) | `app-Debug`, `app-Release`; `app-linux-Debug`, `app-linux-Release` |
| **tests** | `tests.yml` | Windows: ASan, тесты, покрытие (`--min 70`). Linux: `linux-tests` (ASan+UBSan, тесты) | `coverage-report`, `test-report` (Windows) |
| **code-quality** | `code-quality.yml` | Windows: format + tidy. Linux: `linux-tidy` | — |
| **docs** | `docs.yml` | Doxygen + MkDocs; деплой на GitHub Pages из `master` | сайт [shostakovsky.github.io/cpp-app](https://shostakovsky.github.io/cpp-app/) |

## Новый проект по образцу

Каркас нового C++-проекта (CMake, Conan, тесты, анализ, санитайзеры, документация, CI) разворачивается скиллом по спецификации [SCAFFOLD.md](SCAFFOLD.md):

```text
/new-cpp-project <Имя> [каталог]
```

Пример: `/new-cpp-project Sandbox ../sandbox`. Канонический файл скилла — `.claude/skills/new-cpp-project/SKILL.md` (адаптер Antigravity — `.agents/skills.json`).
