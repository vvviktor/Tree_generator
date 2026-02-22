cmake_minimum_required(VERSION 3.10.0)
project(tree_generator VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(../dom dom_lib)

add_library(tree_gen STATIC tree_generator.cpp)

set(PROJECT_INCLUDE_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/../include")

target_include_directories(tree_gen PUBLIC ${PROJECT_INCLUDE_PATHS})

target_link_libraries(tree_gen PUBLIC dom)