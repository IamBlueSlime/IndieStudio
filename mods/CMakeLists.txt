find_library(CWIID_FOUND cwiid)

if (CWIID_FOUND EQUAL TRUE)
    add_subdirectory(WiimoteController)
endif ()