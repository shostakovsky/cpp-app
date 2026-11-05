"""Build the project with AddressSanitizer and run the tests under it.

The asan preset (ENABLE_ASAN=ON) has its own build tree, build/<platform>-asan,
so the regular build stays fast and unchanged. Its workflow preset configures,
builds and tests in one command.

Usage:
    python automation/run_asan.py
"""

from common import preset, run

run(["cmake", "--workflow", "--preset", preset("asan")])
