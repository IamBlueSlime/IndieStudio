find_library(CWIID_FOUND cwiid)

add_subdirectory(MazeWorldGenerator)

if (CWIID_FOUND)
    add_subdirectory(WiimoteController)
endif ()