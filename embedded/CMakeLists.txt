cmake_policy(SET CMP0048 NEW)
cmake_minimum_required(VERSION 3.22)

project(waterer VERSION 3.22)

set(CMAKE_CXX_STANDARD 14)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_definitions(-DARDUINO_TESTING)
add_compile_definitions(ARDUINO_TESTING)

# third party libs

include(FetchContent)

# cmake-lint: disable=C0301
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings

# cmake-lint: disable=C0103
set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# project

add_subdirectory(src)
add_subdirectory(tests)
