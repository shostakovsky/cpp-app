# Обзор

**App** — учебное консольное приложение курса «Инженерный C++». Оно читает
параметры окна из `config.json` (заголовок, ширина, высота) и выводит их в
терминал. На этом небольшом примере отрабатываются воспроизводимая сборка,
тесты, статический анализ, санитайзеры, покрытие кода и непрерывная
интеграция.

Стек проекта: C++23, CMake 3.25+, Conan 2, MSVC (Visual Studio 2022),
Python-скрипты в каталоге `automation/`. Логика разбора конфигурации вынесена
в статическую библиотеку `AppLib`; исполняемый файл `App` остаётся тонкой
точкой входа.

## Быстрый старт

Все команды выполняются из корня репозитория (`project/`):

```powershell
python automation/generate_project_files.py
python automation/build_debug.py
.\build\windows-msvc-debug\bin\App.exe
```

`config.json` читается из текущего рабочего каталога. Сборка кладёт копию
рядом с `App.exe`, поэтому приложение запускается и из корня проекта, и из
`build\windows-msvc-debug\bin`, и из IDE. На Linux — `build/linux-gcc-debug/bin/App`.

Конфигурация Release: `python automation/build_release.py`.
Полная очистка сборочных каталогов: `python automation/clean.py`.

## Разделы портала

| Раздел | Содержание |
| --- | --- |
| [Архитектура](architecture.md) | Цели CMake, `parseConfig`, модули `cmake/` |
| [Разработка](development.md) | Скрипты `automation/`, чек-лист перед коммитом |
| [Справочник API](api/index.html) | Публичный интерфейс, сгенерированный Doxygen |

## Зависимости

Внешние пакеты задаются в `conanfile.txt` (в том числе `nlohmann_json` и
`GTest`). Профили Conan для Debug и Release лежат в `profiles/`; Conan
вызывается из CMake при конфигурации пресета (`cmake/conan_provider.cmake`). Подробности
сборочной модели — на странице [Архитектура](architecture.md).
