cmake_minimum_required(VERSION 2.8.8)

configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(
    ${CMAKE_CURRENT_BINARY_DIR}/googletest-src
    ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
    EXCLUDE_FROM_ALL
)

if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

set_source_files_properties(${INDIE_SRC} PROPERTIES
    COMPILE_FLAGS "-fprofile-arcs -ftest-coverage"
)

add_executable(units
    ${INDIE_SRC}
    ecs/Signature.cpp
)

target_include_directories(units PRIVATE ${INDIE_API_INCLUDE_DIRS})
target_include_directories(units PRIVATE ${INDIE_INCLUDE_DIRS})
target_link_libraries(units ${INDIE_REQUIRED_LIBRARIES} gtest_main gcov)

add_test(NAME bomberman_tests COMMAND units --gtest_output=xml:report.xml)