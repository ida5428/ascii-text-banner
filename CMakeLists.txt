cmake_minimum_required(VERSION 3.16)
project(ascii-text-banner LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(CLI11 INTERFACE)
target_include_directories(CLI11 INTERFACE
    ${CMAKE_SOURCE_DIR}/libs/CLI11/include
)

add_library(tomlplusplus INTERFACE)
target_include_directories(tomlplusplus INTERFACE
    ${CMAKE_SOURCE_DIR}/libs/tomlplusplus/include
)

add_executable(main src/main.cpp src/ascii.cpp src/defaultFont.cpp)

target_link_libraries(main PRIVATE CLI11 tomlplusplus)
