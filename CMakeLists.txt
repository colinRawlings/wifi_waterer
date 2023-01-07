cmake_policy(SET CMP0048 NEW)
cmake_minimum_required(VERSION 3.22)

project(waterer VERSION 3.22)

set(CMAKE_CXX_STANDARD 14)


add_subdirectory(src)
add_subdirectory(test)