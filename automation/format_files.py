"""Format all sources with clang-format, or check them with --check."""

import sys

from common import cmake_build, ensure_configured

ensure_configured("debug")
cmake_build("debug", "format-check" if "--check" in sys.argv[1:] else "format")
