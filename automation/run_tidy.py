"""Run clang-tidy over the project.

clang-tidy takes each file's real compile flags from the compilation database
(compile_commands.json) that every preset exports, so Windows and Linux run
the same command. On Windows clang-tidy ships with Visual Studio (component
"C++ Clang tools for Windows") and is usually not on PATH. Findings are errors
(see .clang-tidy: WarningsAsErrors), so a finding fails the script.

Usage:
    python automation/run_tidy.py
"""

import shutil
from pathlib import Path

from common import IS_WINDOWS, PROJECT_ROOT, build, build_dir, environment, fail, relative, run


def clang_tidy() -> str:
    found = shutil.which("clang-tidy", path=environment().get("PATH"))
    if found:
        return found
    if IS_WINDOWS:
        bundled = Path(environment()["VSINSTALLDIR"], "VC", "Tools", "Llvm", "x64", "bin", "clang-tidy.exe")
        if bundled.is_file():
            return str(bundled)
    fail("clang-tidy not found: install 'C++ Clang tools for Windows' in Visual Studio Installer "
         "or the clang-tidy package on Linux")


build("debug")  # keeps compile_commands.json in step with the CMake files
sources = sorted((PROJECT_ROOT / "src").rglob("*.cpp"))
run([clang_tidy(), "-p", relative(build_dir("debug")), *(relative(s) for s in sources)])
