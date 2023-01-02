add_library(osurs-reserve connection.c reservation.c uuid.c)
target_include_directories(osurs-reserve PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(osurs-reserve PUBLIC)
