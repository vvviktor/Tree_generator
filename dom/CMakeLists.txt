cmake_minimum_required(VERSION 3.10.0)
project(dom LANGUAGES CXX)

add_library(dom STATIC dom.cpp)

set(PROJECT_INCLUDE_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/../include")

target_include_directories(dom PUBLIC ${PROJECT_INCLUDE_PATHS})