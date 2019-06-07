find_library(CWIID_FOUND cwiid)

if (CWIID_FOUND)
    add_subdirectory(WiimoteController)
endif ()