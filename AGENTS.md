# Инструкции для AI-агента (проект App)

Этот репозиторий — кроссплатформенный проект на C++23 (Windows MSVC + Linux GCC 14).

## Сборка и проверки

Все операции сборки, тестирования и анализа выполняются только через скрипты `automation/`:

- `python automation/generate_project_files.py` — установка зависимостей Conan и генерация CMake
- `python automation/build_debug.py` / `python automation/build_release.py` — сборка Debug и Release
- `python automation/run_tests.py` — сборка и запуск модульных тестов GoogleTest через CTest
- `python automation/run_coverage.py --min 70` — проверка тестового покрытия (Windows)
- `python automation/format_files.py` / `python automation/format_files.py --check` — форматирование `clang-format`
- `python automation/run_tidy.py` — статический анализ `clang-tidy`
- `python automation/run_asan.py` — прогон тестов под санитайзерами ASan и UBSan
- `python automation/build_docs.py` — сборка документации Doxygen и MkDocs

## Архитектура

- Общая статическая библиотека `Common` (`src/Common/`) содержит конфигурацию и общие компоненты; её тесты находятся в `src/Common/Tests/` (цель `CommonTests`).
- Исполняемая цель `App` (`src/App/main.cpp`) — тонкая точка входа для чтения конфигурации и обработки ошибок.
- Игра в `src/Game/` разделена на `GameCore` (игровая логика), `GamePresentation` (отображение) и `GameApp` (точка входа); модульные тесты игровой логики собираются в `GameTests`.

## Стандарты кода C++23

- Корневое пространство имён: `LifeExe`. Типы — `PascalCase`, функции, методы, поля и переменные — `camelCase` (приватные поля классов — с префиксом `m_`).
- Унифицированная инициализация фигурными скобками `{}` (`int width{0};`), строчный суффикс `f` для литералов `float` (`0.0f`).
  Исключение — типы с конструктором от `std::initializer_list`, прежде всего `nlohmann::json`: `const auto document = nlohmann::json::parse(input);`.
- `const` по умолчанию, передача строковых параметров для чтения через `std::string_view`.
- Комментарии в исходном коде (C++, CMake, Python) пишутся строго на английском языке и объясняют причину решения (*why*), а не синтаксис.
- Все публичные заголовки документируются комментариями Doxygen (`///`, `///<`, `@param`, `@return`, `@throws`).
- Перед коммитом любой C++ код форматируется через `python automation/format_files.py`.

## Git и доставка изменений

- Ветка `master` защищена (`protection.json`, `enforce_admins: true`): прямой `git push` в `master` запрещён.
- Любые изменения доставляются через отдельную ветку, предкоммитное ревью скиллом `/code-review` и Pull Request (`gh pr create`, `gh pr checks --watch`, `gh pr merge --squash --delete-branch`).
