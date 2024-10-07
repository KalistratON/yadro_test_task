cmake_minimum_required (VERSION 3.8)

project ("tape_sort_algo")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/project_bin_files")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/project_bin_files")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory ("googletest")
add_subdirectory ("Tape")
add_subdirectory ("tests")
add_subdirectory ("SorterExe")

set_target_properties(tests PROPERTIES FOLDER "tests")
set_target_properties(tape PROPERTIES FOLDER "Tape")
set_target_properties(sorterExe PROPERTIES FOLDER "SorterExe")

set_target_properties(gmock         PROPERTIES FOLDER "3rdparty")
set_target_properties(gmock_main    PROPERTIES FOLDER "3rdparty")
set_target_properties(gtest         PROPERTIES FOLDER "3rdparty")
set_target_properties(gtest_main    PROPERTIES FOLDER "3rdparty")

file(COPY config.txt DESTINATION ${PROJECT_BINARY_DIR}/)
file(COPY test_files/ DESTINATION ${PROJECT_BINARY_DIR}/test_files/)