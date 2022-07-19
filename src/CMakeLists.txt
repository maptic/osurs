find_package(LibXml2 REQUIRED)

# Add modules
add_subdirectory(io)
add_subdirectory(network)

# Create automatic library - will be static or dynamic based on user setting
add_library(osurs network/constructor.c network/destructor.c network/getter.c optimize.c io/import.c io/matsim.c io/print.c io/utils.c reserve.c)
target_include_directories(osurs PUBLIC ${PROJECT_SOURCE_DIR}/include ${LIBXML2_INCLUDE_DIR})
target_link_libraries(osurs ${LIBXML2_LIBRARIES})

# Create executable
add_executable(main main.c)
target_link_libraries(main osurs)

