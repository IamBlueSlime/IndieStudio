project(wiimotecontroller VERSION 1.0.0 LANGUAGES CXX)

add_library(wiimotecontroller SHARED
    src/WiimoteController.cpp
    src/WiimoteControlProvider.cpp
)

target_include_directories(wiimotecontroller PRIVATE include)
target_include_directories(wiimotecontroller PRIVATE ${INDIE_API_INCLUDE_DIRS})
target_link_libraries(wiimotecontroller cwiid)