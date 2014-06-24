cmake_minimum_required(VERSION 2.8)

project(smithy)

include(Compiler.cmake)

set(smithy_VERSION_MAJOR 1)
set(smithy_VERSION_MINOR 0)

include_directories(${PROJECT_SOURCE_DIR}/src)
add_subdirectory(src)

include(DebPack.cmake)
