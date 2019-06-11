project(mazeworldgenerator VERSION 1.0.0 LANGUAGES CXX)

add_library(mazeworldgenerator SHARED
    src/MazeWorldGenerator.cpp
)

target_include_directories(mazeworldgenerator PRIVATE include)
target_include_directories(mazeworldgenerator PRIVATE ${INDIE_API_INCLUDE_DIRS})
target_link_libraries(mazeworldgenerator)