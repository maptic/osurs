#include <gtest/gtest.h>

extern "C" {
#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include <osurs/io.h>
}

TEST(IOTest, Import) {
    int success;
    const char* network_file = "input/intercity_network.xml";
    const char* reservation_file = "input/intercity_reservations.xml";
    Network* network = new_network();
    success = import_network(network, network_file);
    EXPECT_EQ(success, 1);
    success = import_reservations(network, reservation_file);
    EXPECT_EQ(success, 1);
    delete_network(network);
}

// Export a network
TEST(IOTest, Export) {
    int success;

    // Osurs format import
    const char* network_file = "input/intercity_network.xml";
    const char* reservation_file = "input/intercity_reservations.xml";
    Network* network = new_network();
    success = import_network(network, network_file);
    EXPECT_EQ(success, 1);
    success = import_reservations(network, reservation_file);
    EXPECT_EQ(success, 1);

    // Osurs format export
    success = export_network(network, "tmp_network_export.xml");
    EXPECT_EQ(success, 1);
    success = export_reservations(network, "tmp_reservation_export.xml");
    EXPECT_EQ(success, 1);
    delete_network(network);

    // MATSim format import and export
    const char* schedule_file = "input/matsim/transitSchedule.xml";
    const char* vehicle_file = "input/matsim/transitVehicles.xml";
    network = new_network();
    success = import_matsim(network, schedule_file, vehicle_file);
    EXPECT_EQ(success, 1);
    success = export_network(network, "tmp_matsim_export.xml");
    EXPECT_EQ(success, 1);
    delete_network(network);
}

/**
 * @brief Read MATSim transit schedule.
 *
 */
TEST(IOTest, ReadMatsim) {
    // File paths
    int success;
    const char* schedule_file = "input/matsim/transitSchedule.xml";
    const char* vehicle_file = "input/matsim/transitVehicles.xml";

    // Init network
    Network* network = new_network();

    // Read files
    success = import_matsim(network, schedule_file, vehicle_file);

    // Test
    EXPECT_EQ(success, 1);
    EXPECT_EQ(network->node_counter, 17);
    EXPECT_EQ(network->route_counter, 2);

    // Check connection: Success
    Node* n1 = get_node(network, "stop1");
    Node* n2 = get_node(network, "stop4");
    Connection* con1 = new_connection(n1, n2, 0);
    ASSERT_TRUE(con1 != NULL);

    // Check connection: Wrong direction
    Connection* con2 = new_connection(n2, n1, 0);
    ASSERT_TRUE(con2 == NULL);

    // Cleanup
    delete_connection(con1);
    delete_connection(con2);
    delete_network(network);
}