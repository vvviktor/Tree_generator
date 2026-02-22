cmake_minimum_required(VERSION 3.10.0)
project(svg_gen LANGUAGES CXX)

add_library(svg_gen STATIC svg.cpp)

set(PROJECT_INCLUDE_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/../include")

target_include_directories(svg_gen PUBLIC ${PROJECT_INCLUDE_PATHS})