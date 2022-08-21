# Add modules
# Note: Note that it is crucial that sub-directories are added in reverse dependency order.
# E.g. osurs-network depends on osurs-io
add_subdirectory(network)
add_subdirectory(io)
add_subdirectory(reserve)
add_subdirectory(reservationTypes)

# Create automatic library - will be static or dynamic based on user setting
add_library(osurs "optimize.cpp")
target_include_directories(osurs PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(osurs osurs-reservationTypes osurs-network osurs-io osurs-reserve)

# Create executable
add_executable(main "main.c")
target_link_libraries(main osurs)

