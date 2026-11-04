"""Build the project with AddressSanitizer and run the tests under it.

The instrumented build lives in its own directory (build-asan) so the regular
build stays fast and unchanged. It reuses the Conan toolchain from the regular
build tree.

Usage:
    python automation/run_asan.py
"""

from common import build_dir, cmake_build, configure, ensure_generated, relative, run

ensure_generated("Debug")
tree = build_dir("Debug", root="build-asan")
configure("Debug", tree, extra=["-DENABLE_ASAN=ON"])
cmake_build(tree, "Debug")
run(["ctest", "--test-dir", relative(tree), "-C", "Debug", "--output-on-failure"])
