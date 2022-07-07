/*
 * Reservation optmization example
 *
 * gcc -W -o osurs main.c network.c routing.c
 *
 * Author:  Merlin Unterfinger
 */

#include <stdio.h>

#include "network.h"
#include "routing.h"

int main(int argc, char *argv[]) {
    // Initialize network
    Network *network = new_network();

    // Nodes
    Node *nodeA = new_node(network, "Albisrieden", 0.0, 0.0);
    Node *nodeB = new_node(network, "Buelach", 1.0, 0.0);
    Node *nodeC = new_node(network, "Chur", 1.0, 1.0);
    Node *nodeD = new_node(network, "Dietikon", 0.0, 1.);
    Node *nodeE = new_node(network, "Nirgendwo", -1.0, -1.0);

    // Route 1
    // Define route attributes
    Node *nodes[] = {nodeA, nodeB, nodeC, nodeD};
    int times[] = {15 * MINUTES, 30 * MINUTES, 45 * MINUTES};
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
    route = new_route(network, nodes2, times2, route_size2,  // Route properties
                      departures2, capacities2, trip_size2   // Trip properties
    );

    // Print network
    printf("TEST: NETWORK\n");
    print_network(network);

    printf("\nTEST: ROUTING\n");
    // Route
    Connection *connection = find_connection(nodeA, nodeD, 12 * HOURS);
    print_connection(connection);

    // One result
    Connection *connection2 =
        find_connection(nodeA, nodeD, 18 * HOURS + 30 * MINUTES);
    print_connection(connection2);

    // No result: Time
    Connection *connection3 = find_connection(nodeA, nodeD, 24 * HOURS);
    print_connection(connection3);

    // No result: No direct route
    Connection *connection4 = find_connection(nodeA, nodeE, 24 * HOURS);
    print_connection(connection4);

    // No result: Same place
    Connection *connection5 = find_connection(nodeA, nodeA, 0 * HOURS);
    print_connection(connection5);

    return 0;
}