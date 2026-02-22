cmake_minimum_required(VERSION 3.10.0)
project(make_tree VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(./tree_generator tree_gen_lib)
add_subdirectory(./tree_renderer tree_rend_lib)

add_executable(make_tree make_tree.cpp)

set(PROJECT_INCLUDE_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_include_directories(make_tree PUBLIC ${PROJECT_INCLUDE_PATHS})

target_link_libraries(make_tree PRIVATE tree_gen tree_rend dom)