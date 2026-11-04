"""Install dependencies for Debug and Release and configure the CMake project."""

from common import CONFIGURATIONS, IS_WINDOWS, conan_install, configure

if IS_WINDOWS:
    # One multi-config tree: both Conan profiles, then a single cmake configure.
    for configuration in CONFIGURATIONS:
        conan_install(configuration)
    configure("Debug")
else:
    # One tree per configuration: install + configure for each.
    for configuration in CONFIGURATIONS:
        conan_install(configuration)
        configure(configuration)
