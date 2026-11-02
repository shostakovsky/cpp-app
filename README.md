# App

![build](https://github.com/shostakovsky/cpp-app/actions/workflows/build.yml/badge.svg)

Учебный проект курса по инженерному C++: консольное приложение App.

## Сборка и запуск

Проект использует менеджер пакетов Conan 2 и мета-систему сборки CMake. Все команды выполняются из каталога проекта (`project/`). Профили и флаги лежат в `automation/common.py`.

```powershell
python automation/generate_project_files.py
python automation/build_debug.py
python automation/build_release.py
python automation/clean.py

# Запуск приложения (файл config.json читается из текущего каталога)
.\build\bin\Debug\App.exe
```

## Архитектура проекта

```text
src/App/
├── CMakeLists.txt   # project(App): цели AppLib (STATIC) и App (EXECUTABLE)
├── Config.h         # публичный контракт библиотеки
├── Config.cpp       # реализация парсинга конфигурации
└── main.cpp         # тонкая точка входа
```

Логика выделена в статическую библиотеку `AppLib`, а исполняемый файл `App` остаётся тонкой обёрткой над ней.

## Тесты

Исходные файлы тестов лежат в `src/App/Tests/`. Они проверяют корректность разбора конфигурационного файла.

```powershell
python automation/run_tests.py
# или напрямую:
.\build\bin\Debug\AppTestRunner.exe
```

### Покрытие кода

```powershell
python automation/run_coverage.py
```

HTML-отчёт: `build/coverage/html/index.html`. В CI минимальный порог покрытия — 70% (`python automation/run_coverage.py --min 70`).

## Непрерывная интеграция (CI)

Сборка автоматизирована через GitHub Actions на виртуальных машинах `windows-latest` (workflow `.github/workflows/build.yml`).

На каждый `push` и pull request выполняются:

- генерация зависимостей (`automation/generate_project_files.py`);
- сборка конфигураций Debug и Release;
- проверка стиля `clang-format` (`automation/format_files.py --check`).

Готовый бинарник публикуется артефактом `app-release` (`build/bin/Release/App.exe` и `config.json`). Его можно скачать в секции **Artifacts** на странице завершённого запуска или командой:

```powershell
gh run download --name app-release --dir ..\from-ci
cd ..\from-ci
.\build\bin\Release\App.exe
```

Запускайте `App.exe` из корня скачанного каталога: программа читает `config.json` из текущего рабочего каталога.

## Стиль кода

стиль зафиксирован в .clang-format (на базе Microsoft);

```powershell
python automation/format_files.py
python automation/format_files.py --check
```

рекомендация включить автоформатирование при сохранении в IDE.

## Статический анализ

Все сборочные цели компилируются с предупреждениями как ошибками (`/W4 /WX`).

Локальный запуск clang-tidy:

```powershell
python automation/run_tidy.py
```

Правила анализатора заданы в `.clang-tidy`.

## AddressSanitizer

Инструментированная сборка с AddressSanitizer живёт в отдельном каталоге `build-asan/`: её нельзя смешивать с обычными объектными файлами из `build/`, а накладные расходы по CPU и памяти делают такую конфигурацию непригодной для повседневной разработки.

Локальный прогон тестов под контролем санитайзера:

```powershell
python automation/run_asan.py
```

В CI шаг `Tests under AddressSanitizer` запускает ту же команду после `Build Debug` и до обычных тестов. Любой выход за границы памяти или use-after-free останавливает пайплайн с подробным отчётом ASan.
