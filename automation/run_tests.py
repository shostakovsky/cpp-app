"""Build the Debug configuration and run all tests through CTest.

Writes a JUnit report to build/<preset>/test-report.xml for CI.
"""

from common import build, preset, run

build("debug")
run(["ctest", "--preset", preset("debug"), "--output-junit", "test-report.xml"])
