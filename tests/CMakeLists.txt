# See: 
# - CTest: https://coderefinery.github.io/cmake-workshop/testing/
# - CTest: https://coderefinery.github.io/cmake-workshop/testing/#test-properties-labels-timeout-and-cost
# - Googletest: https://google.github.io/googletest/quickstart-cmake.html
enable_testing()

# Build test binaries and link googletest and libosurs
add_executable(network_test network_test.cpp ../src/network.c)
target_link_libraries(network_test gtest_main osurs)
add_executable(routing_test routing_test.cpp)
target_link_libraries(routing_test gtest_main osurs)

# GoogleTest
include(GoogleTest)
gtest_discover_tests(network_test)
gtest_discover_tests(routing_test)
