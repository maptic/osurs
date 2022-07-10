cmake_minimum_required(VERSION 3.14.0)

# Project name and language
project(
    osurs
    VERSION 0.1.0
    DESCRIPTION "Optimization of space utilization in reservation systems."
    LANGUAGES C CXX)

# Require C++17 (GoogleTest requires at least C++14)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Testing
include(CTest)
enable_testing()

# Add directories
add_subdirectory(src)
add_subdirectory(tests)

# Pack
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# IDEs should put the headers in a nice place
file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/modern/*.h")
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})
