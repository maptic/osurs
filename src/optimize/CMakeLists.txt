add_library(osurs-optimize optimize.c)
target_include_directories(osurs-optimize PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(osurs-optimize osurs-ds)
