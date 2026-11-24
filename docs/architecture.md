# Архитектура

Проект состоит из общей библиотеки `Common`, тонкого приложения `App` и
игры, разделённой на слои `GameCore`, `GamePresentation` и `GameApp`.
Корневой `CMakeLists.txt` задаёт стандарт C++23, каталоги вывода
(`build/bin`) и подключает общие модули из `cmake/`, после чего добавляет
подкаталоги `src/Common`, `src/App` и `src/Game`.

## Сборочные цели CMake

| Цель | Тип | Исходники | Назначение |
| --- | --- | --- | --- |
| `Common` | `STATIC` | `src/Common/Config.h`, `Config.cpp` | Общий код и разбор конфигурации |
| `CommonTests` | `EXECUTABLE` | `src/Common/Tests/ConfigTest.cpp` | Тесты общей библиотеки (GoogleTest) |
| `App` | `EXECUTABLE` | `src/App/main.cpp` | Тонкая точка входа; использует `Common` |
| `GameCore` | `STATIC` | `src/Game/Core/` | Игровая логика и состояние |
| `GamePresentation` | `STATIC` | `src/Game/Presentation/` | Представление и отрисовка; использует `GameCore` |
| `GameApp` | `EXECUTABLE` | `src/Game/App/main.cpp` | Точка входа игры; связывает игровые слои и `Common` |
| `GameTests` | `EXECUTABLE` | `src/Game/Tests/GameTest.cpp` | Тесты игровой логики (GoogleTest) |

Исполняемые файлы и статические библиотеки попадают в `build/bin/<Config>/`.
`App` читает `config.json` из текущего рабочего каталога; пример конфигурации
находится в корне репозитория, который задан рабочим каталогом отладчика.

## Конфигурация через поток

Библиотека `Common` содержит структуру `LifeExe::GameConfig` и функцию
`LifeExe::parseConfig(std::istream&)`. Функция принимает открытый поток с JSON
вида:

```json
{"title": "App", "window": {"width": 800, "height": 600}}
```

Результат — структура `GameConfig` (`title`, `width`, `height`), объявленная
в `src/Common/Config.h`. При ошибке синтаксиса, отсутствии ключа или неверном
типе выбрасывается `std::runtime_error` с именем проблемного ключа. Такой
контракт не привязан к файловой системе: тесты и приложение могут подавать
строковый поток или файл.

Полная спецификация полей и исключений — в
[справочнике API](api/index.html).

## Модули `cmake/`

| Модуль | Роль |
| --- | --- |
| `CompilerWarnings.cmake` | `set_project_warnings`: `/W4 /WX` (MSVC) или `-Wall -Wextra -Werror` |
| `Sanitizers.cmake` | Опция `ENABLE_ASAN` и `enable_sanitizers` для инструментированной сборки |
| `ClangFormat.cmake` | Цели `format` и `format-check` через `clang-format` |

Предупреждения и санитайзеры применяются к целям всех трёх подкаталогов.
AddressSanitizer включается отдельным деревом
`build-asan/` (см. `automation/run_asan.py`), чтобы не смешивать флаги с
обычным `build/`.
