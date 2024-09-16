cmake_minimum_required(VERSION 3.0)
project(LD19)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_BUILD_TYPE Debug)

aux_source_directory(src SRC_FILES)

add_library(LD19_LIB STATIC ${SRC_FILES})
target_include_directories(LD19_LIB PUBLIC inc)
target_compile_options(LD19_LIB PUBLIC -g)

add_executable(LD19 main.cpp)
target_link_libraries(LD19 LD19_LIB)

target_compile_options(LD19 PUBLIC -g)