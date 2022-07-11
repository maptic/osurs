/*
 * Reservation optmization example
 *
 * gcc -W -o osurs main.c network.c routing.c
 *
 * Author:  Merlin Unterfinger
 */

#include <stdio.h>

#include <osurs/network.h>
#include <osurs/reserve.h>



int main(int argc, char *argv[]) {
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
    printf("TEST: NETWORK\n");
    print_network(network);

    // printf("\nTEST: ROUTING\n");
    // // Route
    // Connection *connection = find(nodeA, nodeD, 12 * HOURS);
    // print_connection(connection);

    // // One result
    // Connection *connection2 = find(nodeA, nodeD, 18 * HOURS + 30 * MINUTES);
    // print_connection(connection2);

    // // No result: Time
    // Connection *connection3 = find(nodeA, nodeD, 24 * HOURS);
    // print_connection(connection3);

    // // No result: No direct route
    // Connection *connection4 = find(nodeA, nodeE, 24 * HOURS);
    // print_connection(connection4);

    // // No result: Same place
    // Connection *connection5 = find(nodeA, nodeA, 0 * HOURS);
    // print_connection(connection5);

    // // Reservation
    // printf("\nTEST: RESERVATION\n");
    // print_connection(connection);
    // printf("Reserve 9: %s\n", reserve(connection, 9) ? "success" : "failure");
    // printf("Reserve 2: %s\n", reserve(connection, 2) ? "success" : "failure");
    // printf("Reserve 2: %s\n", reserve(connection2, 2) ? "success" : "failure");
    // print_connection(find(nodeA, nodeD, 12 * HOURS));

    // Free memory
    delete_network(network);

    return 0;
}