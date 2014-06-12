cmake_minimum_required(VERSION 2.8)
project(smithy)

set(LTO_FLAGS "-flto -fno-use-linker-plugin -fwhole-program")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pthread -std=c++11 -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG -march=native ${LTO_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "-O4 -DNDEBUG -march=native ${LTO_FLAGS}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g ${LTO_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined -O2")
set(CMAKE_EXE_LINKER_FLAGS "${LTO_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${LTO_FLAGS}")

# Set version information in a config.h file
set(smithy_VERSION_MAJOR 0)
set(smithy_VERSION_MINOR 1)
configure_file(
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )
include_directories("${PROJECT_BINARY_DIR}")

set(CMAKE_MODULE_PATH "/usr/share/SFML/cmake/Modules/")

# Check if the compiler supports regex and auto return type deduction
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (NOT (GCC_VERSION VERSION_GREATER 4.8 OR GCC_VERSION VERSION_EQUAL 4.8))
        message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.8 or greater.")
    endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE CLANG_VERSION)
    if (NOT (CLANG_VERSION VERSION_GREATER 3.3 OR CLANG_VERSION VERSION_EQUAL 3.3))
        message(FATAL_ERROR "${PROJECT_NAME} requires clang++ 3.3 or greater.")
    endif ()
else ()
    message(FATAL_ERROR "Your c++ compiler does not support C++11.")
endif ()

# Define sources and executable
set(EXECUTABLE_NAME "smithy")
add_library(shapes shapes.cpp)
add_library(smith smithchart.cpp)
add_executable(${EXECUTABLE_NAME} main.cpp)

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED system window graphics)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(shapes ${SFML_LIBRARIES})
  target_link_libraries(smith ${SFML_LIBRARIES})
endif()

target_link_libraries(smith shapes)
target_link_libraries(smithy smith)

install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)

include(DebPack.cmake)
