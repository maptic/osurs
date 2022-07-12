#include <gtest/gtest.h>

extern "C" {
#include <osurs/network.h>
}

// Create a network
TEST(NetworkTest, Setup) {
    // Initialize network
    Network *network = new_network();

    // Nodes
    Node *nodeA = new_node(network, "Albisrieden", 0.0, 0.0);
    Node *nodeB = new_node(network, "Buelach", 1.0, 0.0);
    Node *nodeC = new_node(network, "Chur", 1.0, 1.0);
    Node *nodeD = new_node(network, "Dietikon", 0.0, 1.0);
    Node *nodeE = new_node(network, "Nirgendwo", -1.0, -1.0);

    // Route 1s
    // Define route attributes (closed, circular route)
    Node *nodes[] = {nodeA, nodeB, nodeC, nodeD, nodeA};
    int times[] = {15 * MINUTES, 30 * MINUTES, 45 * MINUTES, 60 * MINUTES};
    size_t route_size = 5;

    // Define trip attributes
    int departures[] = {6 * HOURS, 9 * HOURS, 12 * HOURS, 15 * HOURS,
                        18 * HOURS};
    int capacities[] = {12, 6, 9, 6, 12};
    size_t trip_size = 5;

    // Create route
    new_route(network, nodes, times, route_size,  // Route properties
              departures, capacities, trip_size   // Trip properties
    );

    // Route 2: Direct and fast
    // Define route attributes
    Node *nodes2[] = {nodeA, nodeD};
    int times2[] = {30 * MINUTES};
    size_t route_size2 = 2;

    // Define trip attributes
    int departures2[] = {9 * HOURS + 30 * MINUTES, 12 * HOURS + 30 * MINUTES,
                         18 * HOURS + 30 * MINUTES};
    int capacities2[] = {9, 6, 9};
    size_t trip_size2 = 3;

    // Create route
    new_route(network, nodes2, times2, route_size2,  // Route properties
              departures2, capacities2, trip_size2   // Trip properties
    );

    // Print network
    // print_network(network);

    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

// Destroy a network
TEST(NetworkTest, Destroy) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}