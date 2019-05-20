cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 17)

project(bomberman VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
add_compile_options(-Wall -Wextra)

add_executable(bomberman
    src/Main.cpp
    src/Bootstraper.cpp
    src/common/Logger.cpp
    src/common/Registry.cpp
    src/ecs/Signature.cpp
    src/ecs/UUID.cpp
    src/mod/LibraryWrapper.cpp
    src/mod/ModManager.cpp
    src/world/World.cpp
    src/world/WorldManager.cpp
    src/utils/CRCUtils.cpp
    # TODO: remove this below
    src/Base.cpp
)

target_include_directories(bomberman PRIVATE include)
target_link_libraries(bomberman dl Irrlicht)