# Add modules
# Note: Note that it is crucial that sub-directories are added in reverse dependency order.
# E.g. osurs-network depends on osurs-io
add_subdirectory(network)
add_subdirectory(io)

# Create automatic library - will be static or dynamic based on user setting
add_library(osurs optimize.c reserve.c)
target_include_directories(osurs PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(osurs osurs-network osurs-io)

# Create executable
add_executable(main main.c)
target_link_libraries(main osurs)

