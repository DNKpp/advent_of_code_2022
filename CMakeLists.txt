cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(advent_of_code_2022)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")
include("get_cpm")
CPMUsePackageLock(package-lock.cmake)

add_subdirectory("src")
