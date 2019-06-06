project(wiimotecontroller VERSION 1.0.0 LANGUAGES CXX)

find_library(CWIID_FOUND cwiid)

if (CWIID_FOUND EQUAL FALSE)
    return ()
endif ()

add_library(wiimotecontroller SHARED
    src/WiimoteController.cpp
)

target_include_directories(wiimotecontroller PRIVATE include)
target_include_directories(wiimotecontroller PRIVATE ${INDIE_API_INCLUDE_DIRS})
target_link_libraries(wiimotecontroller cwiid)