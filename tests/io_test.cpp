#include <gtest/gtest.h>
#include <limits.h>
#include <stdio.h>
#include <unistd.h>

extern "C" {
#include <osurs/io.h>
#include <osurs/reserve.h>
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

TEST(IOTest, Import) {
    EXPECT_EQ(print_cwd(), 1);
    // EXPECT_EQ(print_file("input/matsim/transitSchedule.xml"), 1);
    // EXPECT_EQ(print_file("input/matsim/transitVehicles.xml"), 1);
}

// Export a network
TEST(IOTest, Export) {
    // File paths
    int success;
    const char* schedule_file = "input/matsim/transitSchedule.xml";
    const char* vehicle_file = "input/matsim/transitVehicles.xml";

    // Init network
    Network* network = new_network();

    // Read files
    success = import_matsim(network, schedule_file, vehicle_file);
    print_network(network);

    // Write
    if (success) {
        success = export_network(network, "tmp_export.xml");
    }

    // Check
    EXPECT_EQ(success, 1);
}