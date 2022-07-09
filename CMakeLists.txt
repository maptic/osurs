cmake_minimum_required(VERSION 3.0.0)
# See: https://gitlab.com/CLIUtils/modern-cmake/-/tree/master/examples/extended-project

project(
    osurs
    VERSION 0.1.0
    DESCRIPTION "Optimization of space utilization in reservation systems."
    LANGUAGES C CXX)

# See: https://google.github.io/googletest/quickstart-cmake.html
# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add directories
add_subdirectory(src)
add_subdirectory(tests)

# Includes
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
include(CTest)
include(GoogleTest)

# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.
# Optionally glob, but only for CMake 3.12 or later:
# set(HEADER_LIST "${PROJECT_SOURCE_DIR}/include/osurs/types.h")
file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/modern/*.h")

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})
