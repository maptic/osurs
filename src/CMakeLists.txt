# Add modules
add_subdirectory(ds)
add_subdirectory(network)
add_subdirectory(reserve)
add_subdirectory(io)
add_subdirectory(optimize)
add_subdirectory(olal)

# Create library - will be static (.a .lib) or dynamic (.so .dll .dylib) based on user setting
add_library(osurs SHARED lib.c)
target_include_directories(osurs PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(osurs osurs-ds osurs-network osurs-reserve osurs-io osurs-optimize osurs-olal)

# Install library
install(TARGETS osurs DESTINATION lib)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)

# Create CLI executable and install
add_executable(osurscli cli.c)
target_link_libraries(osurscli osurs)
install(TARGETS osurscli DESTINATION bin)
