find_package(LibXml2 REQUIRED)

# Create automatic library - will be static or dynamic based on user setting
add_library(osurs network.c optimize.c io.c reserve.c)
target_include_directories(osurs PUBLIC ${PROJECT_SOURCE_DIR}/include ${LIBXML2_INCLUDE_DIR})
target_link_libraries(osurs ${LIBXML2_LIBRARIES})

# Create executable
add_executable(main main.c)
target_link_libraries(main osurs)

