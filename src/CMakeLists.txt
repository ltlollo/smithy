set(LTO_FLAGS "-flto -fno-use-linker-plugin -fwhole-program")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pthread -std=c++11 -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG -march=native ${LTO_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -DNDEBUG -march=native ${LTO_FLAGS}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g ${LTO_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined -O2")
set(CMAKE_EXE_LINKER_FLAGS "${LTO_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${LTO_FLAGS}")

configure_file(
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )

set(CMAKE_MODULE_PATH "/usr/share/SFML/cmake/Modules/")

set(EXECUTABLE_NAME "smithy")
add_library(shapes shapes.cpp)
add_library(smith smithchart.cpp)
add_executable(${EXECUTABLE_NAME} main.cpp)

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
