# Add modules
add_subdirectory(ds)
add_subdirectory(network)
add_subdirectory(reserve)
add_subdirectory(io)
add_subdirectory(optimize)
add_subdirectory(olal)

# Create library - will be static or dynamic based on user setting
add_library(osurs lib.c)
target_include_directories(osurs PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(osurs osurs-ds osurs-network osurs-reserve osurs-io osurs-optimize osurs-olal)

# Create executable
add_executable(main main.c)
target_link_libraries(main osurs)
