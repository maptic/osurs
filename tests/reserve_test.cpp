#include <gtest/gtest.h>

extern "C" {
#include <osurs/io.h>
#include <osurs/reserve.h>
}

// Find a connection in the network
TEST(ReserveTest, NewConnection) {
    // Load test network
    Network *network = new_network();
    import_network(network, "input/intercity_network.xml");

    // Create connections
    Node *orig = get_node(network, "Zürich HB");
    Node *dest1 = get_node(network, "Lugano");
    Node *dest2 = get_node(network, "Lausanne");
    Connection *con1 = new_connection(orig, dest1, 60 * 60 * 12);
    EXPECT_TRUE(con1 != NULL);
    Connection *con2 = new_connection(orig, dest2, 60 * 60 * 8);
    EXPECT_TRUE(con2 != NULL);

    // Cleanup
    delete_connection(con1);
    delete_connection(con2);
    delete_network(network);
}

// Check a found connection in the network
TEST(ReserveTest, Check) {
    // Load test network
    Network *network = new_network();
    import_network(network, "input/intercity_network.xml");

    // Create connections
    Node *orig = get_node(network, "Zürich HB");
    Node *dest1 = get_node(network, "Lugano");
    Node *dest2 = get_node(network, "Lausanne");
    Connection *con1 = new_connection(orig, dest1, 60 * 60 * 12);
    Connection *con2 = new_connection(orig, dest2, 60 * 60 * 8);

    // Check
    int trip_count;
    EXPECT_TRUE(check_connection(con1, 1, &trip_count));
    EXPECT_TRUE(check_connection(con2, 10, &trip_count));

    // Cleanup
    delete_connection(con1);
    delete_connection(con2);
    delete_network(network);
}

// Reserve a found connection in the network
TEST(ReserveTest, NewReservation) {
    // Load test network
    Network *network = new_network();
    import_network(network, "input/intercity_network.xml");

    // Create connections
    Node *orig = get_node(network, "Zürich HB");
    Node *dest1 = get_node(network, "Lugano");
    Node *dest2 = get_node(network, "Lausanne");
    Connection *con1 = new_connection(orig, dest1, 60 * 60 * 12);
    Connection *con2 = new_connection(orig, dest2, 60 * 60 * 8);

    // Create reservations
    Reservation *res1 = new_reservation(con1, 2);
    EXPECT_TRUE(res1 != NULL);
    Reservation *res2 = new_reservation(con1, 1);
    EXPECT_TRUE(res2 != NULL);
    Reservation *res3 = new_reservation(con1->next->next, 5);
    EXPECT_TRUE(res3 != NULL);
    Reservation *res4 = new_reservation(con2, 1);
    EXPECT_TRUE(res4 != NULL);
    Reservation *res5 = new_reservation(con2, 1);
    EXPECT_TRUE(res5 != NULL);
    Reservation *res6 = new_reservation(con2, 1);
    EXPECT_TRUE(res6 != NULL);
    Reservation *res7 = new_reservation(con2->next, 10);
    EXPECT_TRUE(res7 != NULL);
    Reservation *res8 = new_reservation(con2->next->next, 7);
    EXPECT_TRUE(res8 != NULL);

    // Expect failing
    Reservation *res9 = new_reservation(con2->next->next, INT_MAX);
    EXPECT_TRUE(res9 == NULL);
    Reservation *res10 = new_reservation(NULL, 1);
    EXPECT_TRUE(res10 == NULL);

    // Export test data
    // export_reservations(network, "intercity_reservations.xml");

    // Cleanup
    delete_connection(con1);
    delete_connection(con2);
    delete_network(network);
}