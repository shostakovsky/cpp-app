# App

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

## Стиль кода

стиль зафиксирован в .clang-format (на базе Microsoft);

```powershell
python automation/format_files.py
python automation/format_files.py --check
```

рекомендация включить автоформатирование при сохранении в IDE.
