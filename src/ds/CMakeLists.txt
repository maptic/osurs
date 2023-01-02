add_library(osurs-ds arraylist.c hashmap.c linkedlist.c priority.c queue.c stack.c)
target_include_directories(osurs-ds PUBLIC ${PROJECT_SOURCE_DIR}/include)
