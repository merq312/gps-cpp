cmake_minimum_required(VERSION 3.21)
project(jca-gps)

# ////////////////////////////////////////////////////////

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_compile_options(-Wall -Wextra -pedantic -Werror)

# TESTING ////////////////////////////////////////////////

find_package(Catch2 REQUIRED)
add_executable(tests test.cpp gps.cpp)
target_link_libraries(tests Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(tests)

# MAIN ///////////////////////////////////////////////////

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(gps main.cpp gps.cpp)
target_link_libraries(gps ${CONAN_LIBS})
