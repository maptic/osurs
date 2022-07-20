add_library(osurs-network constructor.c destructor.c getter.c)
target_include_directories(osurs-network PUBLIC ${PROJECT_SOURCE_DIR}/include)
