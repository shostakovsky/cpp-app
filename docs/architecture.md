# Архитектура

Проект разделён на библиотеку с доменной логикой, тонкое приложение и
отдельный тестовый раннер. Корневой `CMakeLists.txt` задаёт стандарт C++23,
каталоги вывода (`build/bin`) и подключает общие модули из `cmake/`, после
чего добавляет подкаталог `src/App`.

## Сборочные цели CMake

| Цель | Тип | Исходники | Назначение |
| --- | --- | --- | --- |
| `AppLib` | `STATIC` | `Config.h`, `Config.cpp` | Разбор конфигурации и публичный API |
| `App` | `EXECUTABLE` | `main.cpp` | Точка входа; линкуется с `AppLib` |
| `AppTestRunner` | `EXECUTABLE` | `Tests/ConfigTest.cpp` | Юнит-тесты на GoogleTest (`gtest_main`) |

Исполняемые файлы и статические библиотеки попадают в `build/bin/<Config>/`.
Для `App` задан `VS_DEBUGGER_WORKING_DIRECTORY` на корень репозитория, чтобы
отладчик находил `config.json`.

## Конфигурация через поток

Публичный контракт библиотеки — пространство имён `LifeExe` и функция
`parseConfig(std::istream&)`. Вход — открытый поток с JSON вида:

```json
{"title": "App", "window": {"width": 800, "height": 600}}
```

Результат — структура `AppConfig` (`title`, `width`, `height`). При ошибке
синтаксиса, отсутствии ключа или неверном типе выбрасывается
`std::runtime_error` с именем проблемного ключа. Такой контракт не привязан к
файловой системе: тесты и приложение могут подавать строковый поток или
файл.

Полная спецификация полей и исключений — в
[справочнике API](api/index.html).

## Модули `cmake/`

| Модуль | Роль |
| --- | --- |
| `CompilerWarnings.cmake` | `set_project_warnings`: `/W4 /WX` (MSVC) или `-Wall -Wextra -Werror` |
| `Sanitizers.cmake` | Опция `ENABLE_ASAN` и `enable_sanitizers` для инструментированной сборки |
| `ClangFormat.cmake` | Цели `format` и `format-check` через `clang-format` |

Предупреждения и санитайзеры применяются ко всем трём целям в
`src/App/CMakeLists.txt`. AddressSanitizer включается отдельным деревом
`build-asan/` (см. `automation/run_asan.py`), чтобы не смешивать флаги с
обычным `build/`.
