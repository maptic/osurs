find_package(LibXml2 REQUIRED)
add_library(osurs-io export.c import.c matsim.c print.c utils.c)
target_include_directories(osurs-io PUBLIC ${PROJECT_SOURCE_DIR}/include ${LIBXML2_INCLUDE_DIR})
target_link_libraries(osurs-io ${LIBXML2_LIBRARIES} osurs-network osurs-reserve)
