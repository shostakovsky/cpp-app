project(App LANGUAGES CXX)

add_library(${PROJECT_NAME}Lib STATIC Config.cpp Config.h)
target_include_directories(${PROJECT_NAME}Lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME}Lib PRIVATE nlohmann_json::nlohmann_json)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}Lib)

set_target_properties(${PROJECT_NAME}Lib ${PROJECT_NAME} PROPERTIES FOLDER "${PROJECT_NAME}")
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

# Tests: link the same library the app uses; gtest_main provides main().
add_executable(${PROJECT_NAME}TestRunner Tests/ConfigTest.cpp)
target_link_libraries(${PROJECT_NAME}TestRunner PRIVATE ${PROJECT_NAME}Lib GTest::gtest_main)
set_target_properties(${PROJECT_NAME}TestRunner PROPERTIES FOLDER ${PROJECT_NAME})

set_project_warnings(${PROJECT_NAME}Lib)
set_project_warnings(${PROJECT_NAME})
set_project_warnings(${PROJECT_NAME}TestRunner)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}TestRunner DISCOVERY_TIMEOUT 30)
