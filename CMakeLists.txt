cmake_minimum_required(VERSION 2.8)
project(daemon)

set(CMAKE_CXX_STANDARD 11)
include_directories(test)
include_directories(src)
aux_source_directory(test TEST)
aux_source_directory(src SRC)

add_executable(daemon ${SRC} ${TEST})