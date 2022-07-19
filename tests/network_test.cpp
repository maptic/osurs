#include <gtest/gtest.h>

extern "C" {
#include <osurs/network.h>
}

/**
 * @brief Test network creation.
 *
 */
TEST(NetworkTest, Create) {
    // Initialize network
    Network *network = new_network();

    // Nodes
    Node *nodeA = new_node(network, "Albisrieden", 0.0, 0.0);
    Node *nodeB = new_node(network, "Buelach", 1.0, 0.0);
    Node *nodeC = new_node(network, "Chur", 1.0, 1.0);
    Node *nodeD = new_node(network, "Dietikon", 0.0, 1.0);
    Node *nodeE = new_node(network, "Nirgendwo", -1.0, -1.0);
    EXPECT_EQ(network->node_counter, 5);

    // Compositions
    Composition *train = new_composition(network, "train", 500);
    Composition *bus = new_composition(network, "bus", 50);
    EXPECT_EQ(network->composition_counter, 2);

    // Vehicles: Train
    Vehicle *v1 = new_vehicle(network, "rt-1", train);
    Vehicle *v2 = new_vehicle(network, "rt-2", train);
    Vehicle *v3 = new_vehicle(network, "rt-3", train);
    Vehicle *v4 = new_vehicle(network, "rt-4", train);
    Vehicle *v5 = new_vehicle(network, "rt-5", train);
    EXPECT_EQ(network->vehicle_counter, 5);
    // Bus
    Vehicle *v6 = new_vehicle(network, "bb-1", bus);
    Vehicle *v7 = new_vehicle(network, "bb-2", bus);
    Vehicle *v8 = new_vehicle(network, "bb-3", bus);
    EXPECT_EQ(network->vehicle_counter, 8);

    // Route 1s
    // Define route attributes (closed, circular route)
    const char *route_id = "blue";
    Node *nodes[] = {nodeA, nodeB, nodeC, nodeD, nodeA};
    int arrival_offsets[] = {0, 15 * MINUTES, 25 * MINUTES, 40 * MINUTES,
                             60 * MINUTES};
    int departure_offsets[] = {0, 20 * MINUTES, 30 * MINUTES, 45 * MINUTES,
                               60 * MINUTES};
    size_t route_size = 5;

    // Define trip attributes
    const char *trip_ids[] = {"blue-1", "blue-2", "blue-3", "blue-4", "blue-5"};
    int departures[] = {6 * HOURS, 9 * HOURS, 12 * HOURS, 15 * HOURS,
                        18 * HOURS};
    Vehicle *vehicles[] = {v1, v2, v3, v4, v5};
    size_t trip_size = 5;

    // Create route
    new_route(network, route_id, nodes, arrival_offsets,
              departure_offsets,  // Route properties
              route_size, trip_ids, departures, vehicles,
              trip_size  // Trip properties
    );
    EXPECT_EQ(network->route_counter, 1);
    for (size_t i = 0; i < route_size; ++i) {
        EXPECT_EQ(nodes[i]->route_counter, 1);
    }

    // Route 2: Direct and fast
    // Define route attributes
    const char *route_id2 = "red";
    Node *nodes2[] = {nodeA, nodeD};
    int arrival_offsets2[] = {0, 30 * MINUTES};
    int departure_offsets2[] = {0, 30 * MINUTES};
    size_t route_size2 = 2;

    // Define trip attributes
    const char *trip_ids2[] = {"red-1", "red-2", "red-3",
                               "red-4", "red-5", "red-6"};
    int departures2[] = {9 * HOURS + 30 * MINUTES, 12 * HOURS + 30 * MINUTES,
                         18 * HOURS + 30 * MINUTES};
    Vehicle *vehicles2[] = {v6, v7, v8};
    size_t trip_size2 = 3;

    // Create route
    new_route(network, route_id2, nodes2, arrival_offsets2,
              departure_offsets2,  // Route properties
              route_size2, trip_ids2, departures2, vehicles2,
              trip_size2  // Trip properties
    );
    EXPECT_EQ(network->route_counter, 2);
    for (size_t i = 0; i < route_size2; ++i) {
        EXPECT_EQ(nodes2[i]->route_counter, 2);
    }

    // Free memory
    delete_network(network);
}

/**
 * @brief Test network destruction.
 *
 */
TEST(NetworkTest, Destroy) {
    // Initialize network
    Network *network = new_network();

    // Free memory of all structures in the memory and the network itself
    delete_network(network);

    // valgrind ./network_test
    EXPECT_TRUE(1);
}