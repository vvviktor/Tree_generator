cmake_minimum_required(VERSION 3.10.0)
project(tree_renderer VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(../svg svg_gen_lib)

add_library(tree_rend STATIC tree_renderer.cpp)

set(PROJECT_INCLUDE_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/../include")

target_include_directories(tree_rend PUBLIC ${PROJECT_INCLUDE_PATHS})

target_link_libraries(tree_rend PUBLIC svg_gen)