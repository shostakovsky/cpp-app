"""Configure the Debug and Release build trees from CMakePresets.json.

Conan installs the dependencies during the configure of each tree.
"""

from common import configure

for variant in ("debug", "release"):
    configure(variant)
