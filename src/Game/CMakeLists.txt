add_library(GameCore STATIC
	Core/Types.h
	Core/GameSnapshot.h
	Core/Game.h
	Core/Game.cpp
	Core/Command.h
)
target_include_directories(GameCore PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/Core
)

add_library(GamePresentation STATIC
	Presentation/ConsoleRenderer.h
	Presentation/ConsoleRenderer.cpp
)
target_link_libraries(GamePresentation PUBLIC GameCore)
target_include_directories(GamePresentation PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/Presentation
)

add_executable(GameApp
	App/main.cpp
	App/Timer.h
)
target_link_libraries(GameApp PRIVATE GamePresentation GameCore Common)
set_property(TARGET GameApp PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

add_executable(GameTests
	Tests/GameTest.cpp
)
target_link_libraries(GameTests PRIVATE GameCore GTest::gtest_main)

set(_game_targets GameCore GamePresentation GameApp GameTests)
foreach(_game_target IN LISTS _game_targets)
	set_project_warnings(${_game_target} ENABLE_WARNINGS_AS_ERRORS)
	enable_sanitizers(${_game_target})
	set_property(TARGET ${_game_target} PROPERTY FOLDER Game)
endforeach()

include(GoogleTest)
gtest_discover_tests(GameTests DISCOVERY_TIMEOUT 30)
