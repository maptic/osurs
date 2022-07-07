/*
 * Reservation optmization example
 *
 * gcc -o osurs main.c network.c
 *
 * Author:  Merlin Unterfinger
 */

#include "network.h"

int main(int argc, char *argv[]) {
    // Initialize network
    Network *network = new_network();

    // Nodes
    Node *nodeA = new_node(network, "Albisrieden", 0.0, 0.0);
    Node *nodeB = new_node(network, "Buelach", 1.0, 0.0);
    Node *nodeC = new_node(network, "Chur", 1.0, 1.0);
    Node *nodeD = new_node(network, "Dietikon", 0.0, 1.);

    // Define route attributes
    Node *nodes[] = {nodeA, nodeB, nodeC, nodeD};
    int times[] = {15 * MINUTES, 30 * MINUTES, 45 * MINUTES, 60 * MINUTES};
    size_t route_size = 4;

    // Define trip attributes
    int departures[] = {6 * HOURS, 9 * HOURS, 12 * HOURS, 15 * HOURS,
                        18 * HOURS};
    int capacities[] = {12, 6, 9, 6, 12};
    size_t trip_size = 5;

    // Create route
    Route *route =
        new_route(network, nodes, times, route_size,  // Route properties
                  departures, capacities, trip_size   // Trip properties
        );

    // Print network
    print_network(network);

    return 0;
}