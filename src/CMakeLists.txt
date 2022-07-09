# Make an automatic library - will be static or dynamic based on user setting
add_library(osurs network.c routing.c ${HEADER_LIST})
# We need this directory, and users of our library will need it too
target_include_directories(osurs PUBLIC ../include)

# Create executable
add_executable(main main.c network.c routing.c ${HEADER_LIST})
target_include_directories(main PUBLIC ../include)
