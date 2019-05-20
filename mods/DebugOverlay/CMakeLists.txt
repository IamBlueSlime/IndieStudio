add_library(DebugOverlay SHARED
    src/DebugOverlay.cpp
)

target_include_directories(DebugOverlay PRIVATE include)
target_include_directories(DebugOverlay PRIVATE ${INDIE_INCLUDE_DIRS})