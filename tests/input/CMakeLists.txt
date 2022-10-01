# Add subdirectories
add_subdirectory(matsim)

# Add files
configure_file(intercity_network.xml intercity_network.xml COPYONLY)
configure_file(intercity_reservations.xml intercity_reservations.xml COPYONLY)
