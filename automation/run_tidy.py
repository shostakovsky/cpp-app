"""Run clang-tidy over the project.

On Windows, Visual Studio runs clang-tidy through MSBuild code analysis and
passes each file's real compile flags, so no compile_commands.json is needed.
On Linux, clang-tidy reads the compilation database produced by CMake
(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON). Findings are errors (see .clang-tidy:
WarningsAsErrors), so a finding fails the script.

Usage:
    python automation/run_tidy.py
"""

from common import (
    IS_WINDOWS,
    PROJECT_ROOT,
    build_dir,
    cmake_build,
    ensure_generated,
    relative,
    run,
)

ensure_generated("Debug")

if IS_WINDOWS:
    run([
        "cmake", "--build", relative(build_dir("Debug")), "--config", "Debug",
        "--",
        "/p:RunCodeAnalysis=true",
        "/p:EnableClangTidyCodeAnalysis=true",
        "/p:EnableMicrosoftCodeAnalysis=false",
    ])
else:
    cmake_build(build_dir("Debug"), "Debug")  # generated headers and the database must exist
    sources = sorted((PROJECT_ROOT / "src").rglob("*.cpp"))
    run(["clang-tidy", "-p", relative(build_dir("Debug")), *(relative(s) for s in sources)])
