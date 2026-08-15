cmake_minimum_required(VERSION 4.4.2)
project(PhysBox2D)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_RPATH "$ORIGIN/lib")
set(CMAKE_INSTALL_RPATH "$ORIGIN/lib")

include(FetchContent)

message(STATUS "Fetching SFML 3.1.0...")
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SFML)

if(MSVC)
    add_compile_options("/Zc:__cplusplus")
endif()


message(STATUS "Fetching Box2D...")
FetchContent_Declare(
    box2d
    GIT_REPOSITORY https://github.com/erincatto/box2d.git
    GIT_TAG main
)
set(BOX2D_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(box2d)

set(SOURCES
    "src/Game/main.cpp"
    "src/Game/Game.cpp"
    "src/Game/Entities/Player.cpp"
    "src/Game/World/Map.cpp"
    "src/Game/States/MenuState.cpp"
    "src/Game/States/GameState.cpp"
    "src/Game/States/PauseState.cpp"
    "src/Engine/GUI/Button.cpp"
)
add_executable(PhysBox2D ${SOURCES})

target_include_directories(PhysBox2D PRIVATE include)
target_link_libraries(PhysBox2D PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
    box2d
)

if(WIN32)
    add_custom_command(TARGET PhysBox2D POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/lib"
        "$<TARGET_FILE_DIR:PhysBox2D>"
    )
endif()

add_custom_command(TARGET PhysBox2D POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:PhysBox2D>/assets"
)
