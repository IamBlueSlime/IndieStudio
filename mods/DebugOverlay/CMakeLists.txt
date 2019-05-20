project(debugoverlay VERSION 1.0.0 LANGUAGES CXX)

add_library(debugoverlay SHARED
    src/DebugOverlay.cpp
)

target_include_directories(debugoverlay PRIVATE include)
target_include_directories(debugoverlay PRIVATE ${INDIE_API_INCLUDE_DIRS})