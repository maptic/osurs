add_library(osurs-reserve connection.c reservation.c)
target_include_directories(osurs-reserve PUBLIC ${PROJECT_SOURCE_DIR}/include)
