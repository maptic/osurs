add_library(osurs-olal olal.c)
target_include_directories(osurs-olal PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(osurs-olal osurs-optimize)
