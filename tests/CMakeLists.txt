cmake_minimum_required(VERSION 3.8)

add_executable(tests "main.cpp")

if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/MT")
    set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
endif()

target_link_libraries(tests PRIVATE tape)
target_link_libraries(tests PRIVATE gtest gmock)

target_include_directories(tests PRIVATE ${tape_SOURCE_DIR}/tape/inc)
target_include_directories(tests PRIVATE ${gtest_SOURCE_DIR}/include)
target_include_directories(tests PRIVATE ${gmock_SOURCE_DIR}/include)