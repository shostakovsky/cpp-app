"""Common helpers for the project automation scripts.

All scripts run from any directory: paths are computed from this file,
not from the current working directory.

Build settings live in CMakePresets.json; the scripts only choose a preset
and pass it to cmake, cmake --build and ctest, as an IDE does. A preset is
named <platform>-<variant> (windows-msvc-debug, linux-gcc-asan, ...), its build
tree is build/<preset> and executables land in build/<preset>/bin. Conan
installs the dependencies on the first configure of a tree, through
cmake/conan_provider.cmake.

The Ninja generator needs the MSVC tools (cl.exe, the Windows SDK) in the
environment, which a Developer Command Prompt sets up. Started from a plain
console, the scripts load that environment from Visual Studio themselves.
"""

from __future__ import annotations

import json
import os
import shutil
import subprocess
import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parent.parent
IS_WINDOWS = sys.platform == "win32"
PLATFORM = "windows-msvc" if IS_WINDOWS else "linux-gcc"

VSWHERE = Path(os.environ.get("ProgramFiles(x86)", r"C:\Program Files (x86)"), "Microsoft Visual Studio", "Installer", "vswhere.exe")

_environment: dict[str, str] | None = None


def fail(message: str) -> None:
    print(message, file=sys.stderr)
    sys.exit(1)


def preset(variant: str) -> str:
    """Preset for this platform; variant is debug, release or asan."""
    return f"{PLATFORM}-{variant}"


def build_dir(variant: str) -> Path:
    return PROJECT_ROOT / "build" / preset(variant)


def bin_dir(variant: str) -> Path:
    return build_dir(variant) / "bin"


def relative(path: Path) -> str:
    """Path relative to the project root, forward slashes."""
    return path.relative_to(PROJECT_ROOT).as_posix()


def _msvc_environment() -> dict[str, str]:
    """Environment of a Developer Command Prompt for x64, read from vcvars64.bat."""
    if not VSWHERE.is_file():
        fail(f"Visual Studio not found: {VSWHERE} is missing")
    found = subprocess.run(
        [str(VSWHERE), "-latest", "-products", "*",
         "-requires", "Microsoft.VisualStudio.Component.VC.Tools.x86.x64",
         "-property", "installationPath"],
        capture_output=True, text=True)
    installation = found.stdout.strip()
    if not installation:
        fail("Visual Studio with the C++ x64 tools not found (vswhere)")
    vcvars = Path(installation, "VC", "Auxiliary", "Build", "vcvars64.bat")
    # Python prints the environment as ASCII-only JSON: the output of cmd's own
    # 'set' depends on the console code page and garbles non-ASCII paths.
    dump = f'"{sys.executable}" -c "import json, os; print(json.dumps(dict(os.environ)))"'
    result = subprocess.run(f'"{vcvars}" >nul && {dump}', shell=True, capture_output=True, text=True)
    if result.returncode != 0:
        fail(f"{vcvars} failed:\n{result.stdout}{result.stderr}")
    return json.loads(result.stdout.splitlines()[-1])


def environment() -> dict[str, str]:
    """Environment for the tools: on Windows it always has the MSVC tools."""
    global _environment
    if _environment is None:
        if IS_WINDOWS and "VSINSTALLDIR" not in os.environ:
            print("loading the MSVC environment from Visual Studio", flush=True)
            _environment = _msvc_environment()
        else:
            _environment = dict(os.environ)
    return _environment


def run(command: list[str]) -> None:
    """Run a command from the project root; stop the script if it fails."""
    print("+", " ".join(command), flush=True)
    env = environment()
    # Windows looks the program up on the PATH of this process, not of env.
    program = shutil.which(command[0], path=env.get("PATH")) or command[0]
    result = subprocess.run([program, *command[1:]], cwd=PROJECT_ROOT, env=env)
    if result.returncode != 0:
        print(f"command failed with exit code {result.returncode}", file=sys.stderr)
        sys.exit(result.returncode)


def configure(variant: str) -> None:
    run(["cmake", "--preset", preset(variant)])


def ensure_configured(variant: str) -> None:
    """Configure a build tree if it does not exist yet."""
    if (build_dir(variant) / "CMakeCache.txt").exists():
        return
    print("build directory not found, generating project files first")
    configure(variant)


def cmake_build(variant: str, target: str | None = None) -> None:
    command = ["cmake", "--build", "--preset", preset(variant)]
    if target:
        command += ["--target", target]
    run(command)


def build(variant: str) -> None:
    ensure_configured(variant)
    cmake_build(variant)
