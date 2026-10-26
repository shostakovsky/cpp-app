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
.\build\Debug\app.exe
```

## Непрерывная интеграция (CI)

Сборка автоматизирована через GitHub Actions на виртуальных машинах `windows-latest` (workflow `.github/workflows/build.yml`).

На каждый `push` и pull request выполняются:

- генерация зависимостей (`automation/generate_project_files.py`);
- сборка конфигураций Debug и Release;
- проверка стиля `clang-format` (`automation/format_files.py --check`).

Готовый бинарник публикуется артефактом `app-release` (`build/Release/app.exe` и `config.json`). Его можно скачать в секции **Artifacts** на странице завершённого запуска или командой:

```powershell
gh run download --name app-release --dir ..\from-ci
cd ..\from-ci
.\build\Release\app.exe
```

Запускайте `app.exe` из корня скачанного каталога: программа читает `config.json` из текущего рабочего каталога.

## Стиль кода

стиль зафиксирован в .clang-format (на базе Microsoft);

```powershell
python automation/format_files.py
python automation/format_files.py --check!!!!
```

рекомендация включить автоформатирование при сохранении в IDE.

<!-- CI test run for ConEmu colors -->
<!-- CI test run 2 -->
