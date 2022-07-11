# Create automatic library - will be static or dynamic based on user setting
add_library(osurs network.c reserve.c optimize.c)
target_include_directories(osurs PUBLIC ../include)

# Create executable
add_executable(main main.c network.c reserve.c optimize.c)
target_include_directories(main PUBLIC ../include)
