include(FetchContent)

# Get GoogleTest from GitHub
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Build test binaries and link googletest and libosurs
add_executable(network_test network_test.cpp)
target_link_libraries(network_test PRIVATE gtest_main osurs)
add_executable(optimize_test optimize_test.cpp)
target_link_libraries(optimize_test PRIVATE gtest_main osurs)
add_executable(routing_test routing_test.cpp)
target_link_libraries(routing_test PRIVATE gtest_main osurs)

# Discover tests
include(GoogleTest)
gtest_discover_tests(network_test)
gtest_discover_tests(optimize_test)
gtest_discover_tests(routing_test)
