# Разработка

Локальная сборка, тесты и проверки качества запускаются теми же скриптами, что
и CI — одни и те же файлы `automation/` работают и на Windows, и на Linux
(`python` / `python3`). Общие пути и вызов subprocess собраны в
`automation/common.py`: скрипты можно вызывать из любого текущего каталога.

Раскладка сборочных деревьев различается из‑за генераторов CMake:

| Платформа | Каталог конфигурации | Бинарники |
| --- | --- | --- |
| Windows (MSVC, multi-config) | `build/` | `build/bin/<Config>/` |
| Linux (GCC, single-config) | `build/<Config>/` | `build/<Config>/bin/` |

## Скрипты `automation/`

| Скрипт | Назначение |
| --- | --- |
| `generate_project_files.py` | Conan install + генерация CMake (Debug/Release) |
| `build_debug.py` | Сборка конфигурации Debug |
| `build_release.py` | Сборка конфигурации Release |
| `clean.py` | Удаление сборочных каталогов (`--dry-run` — только список) |
| `format_files.py` | Форматирование; `--check` — только проверка стиля |
| `run_tests.py` | Debug + CTest; JUnit в `build/test-report.xml` |
| `run_coverage.py` | Покрытие OpenCppCoverage; опционально `--min N` |
| `run_asan.py` | Сборка в `build-asan/` с ASan и прогон тестов |
| `run_tidy.py` | Статический анализ clang-tidy |
| `build_docs.py` | Документация проекта. Doxygen + MkDocs |

Конфигурации стиля: `.clang-format`, `.clang-tidy`. Профили Conan:
`profiles/windows-msvc-{debug,release}`, `profiles/linux-gcc-{debug,release}`.

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
