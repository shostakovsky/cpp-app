project(Common LANGUAGES CXX)

add_library(Common STATIC Config.cpp Config.h)
target_include_directories(Common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Common PRIVATE nlohmann_json::nlohmann_json)

add_executable(CommonTests Tests/ConfigTest.cpp)
target_link_libraries(CommonTests PRIVATE Common GTest::gtest_main)

set_project_warnings(Common)
set_project_warnings(CommonTests)
enable_sanitizers(Common)
enable_sanitizers(CommonTests)
set_target_properties(Common CommonTests PROPERTIES FOLDER Common)

include(GoogleTest)
gtest_discover_tests(CommonTests DISCOVERY_TIMEOUT 30)
