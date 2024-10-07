cmake_minimum_required(VERSION 3.8)

add_executable(sorterExe "main.cpp")

target_link_libraries(sorterExe PRIVATE tape)

target_include_directories(sorterExe PRIVATE ${tape_SOURCE_DIR}/tape/inc)