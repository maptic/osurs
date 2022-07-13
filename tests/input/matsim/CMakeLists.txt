# If the input file is modified the build system will re-run CMake
# to re-configure the file and generate the build system again.
configure_file(README.md README.md COPYONLY)
configure_file(transitSchedule.xml transitSchedule.xml COPYONLY)
configure_file(vehicles.xml vehicles.xml COPYONLY)