cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/mods)

# Irrlicht support either for Linux and Windows
set(Irrlicht_DIR "vendor/Irrlicht")
include(FindIrrlicht)
find_package(Irrlicht)
include_directories(${Irrlicht_INCLUDE_DIRS})
message("-- Found Irrlicht include directories: " ${Irrlicht_INCLUDE_DIRS})
message("-- Found Irrlicht libraries: " ${Irrlicht_LIBRARIES})

option(ENABLE_GAME "Enable build for the game" ON)
option(ENABLE_MODS "Enable build for the mods" ON)

set(INDIE_INCLUDE_DIRS include)

if (ENABLE_GAME)
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

    target_include_directories(bomberman PRIVATE ${INDIE_INCLUDE_DIRS})
    target_link_libraries(bomberman ${Irrlicht_LIBRARIES} stdc++fs)

    if (UNIX)
        target_link_libraries(bomberman dl)
    endif ()
endif ()

if (ENABLE_MODS)
    add_subdirectory(mods)
endif ()
