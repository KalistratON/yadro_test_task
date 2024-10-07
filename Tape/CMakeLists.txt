cmake_minimum_required(VERSION 3.8)

project(tape)

SET(CMAKE_CXX_STANDARD 17)

file(GLOB tape "${CMAKE_CURRENT_SOURCE_DIR}/*/*")

add_library(tape SHARED ${tape})
target_include_directories(tape PUBLIC ${tape_SOURCE_DIR}/inc)