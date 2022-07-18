/*
 * Reservation optmization example
 *
 * gcc -W -o osurs main.c network.c routing.c
 *
 * Author:  Merlin Unterfinger
 */

#include <osurs/io.h>
#include <osurs/reserve.h>

int main(int argc, char *argv[]) {
    printf("TEST: NETWORK\n");

    // Initialize network
    Network *network = new_network();

    // Nodes
    Node *nodeA = new_node(network, "Albisrieden", 0.0, 0.0);
    Node *nodeB = new_node(network, "Buelach", 1.0, 0.0);
    Node *nodeC = new_node(network, "Chur", 1.0, 1.0);
    Node *nodeD = new_node(network, "Dietikon", 0.0, 1.0);
    Node *nodeE = new_node(network, "Nirgendwo", -1.0, -1.0);

    // Compositions
    Composition *train = new_composition(network, "train", 500);
    Composition *bus = new_composition(network, "bus", 50);

    // Vehicles: Train
    Vehicle *v1 = new_vehicle(network, "rt-1", train);
    Vehicle *v2 = new_vehicle(network, "rt-2", train);
    Vehicle *v3 = new_vehicle(network, "rt-3", train);
    Vehicle *v4 = new_vehicle(network, "rt-4", train);
    Vehicle *v5 = new_vehicle(network, "rt-5", train);
    // Bus
    Vehicle *v6 = new_vehicle(network, "bb-1", bus);
    Vehicle *v7 = new_vehicle(network, "bb-2", bus);
    Vehicle *v8 = new_vehicle(network, "bb-3", bus);

    // Route 1s
    // Define route attributes (closed, circular route)
    const char *route_id = "blue";
    Node *nodes[] = {nodeA, nodeB, nodeC, nodeD, nodeA};
    int times[] = {15 * MINUTES, 30 * MINUTES, 45 * MINUTES, 60 * MINUTES};
    size_t route_size = 5;

    // Define trip attributes
    const char *trip_ids[] = {"blue-1", "blue-2", "blue-3", "blue-4", "blue-5"};
    int departures[] = {6 * HOURS, 9 * HOURS, 12 * HOURS, 15 * HOURS,
                        18 * HOURS};
    Vehicle *vehicles[] = {v1, v2, v3, v4, v5};
    size_t trip_size = 5;

    // Create route
    new_route(network, route_id, nodes, times, route_size,  // Route properties
              trip_ids, departures, vehicles, trip_size     // Trip properties
    );

    // Route 2: Direct and fast
    // Define route attributes
    const char *route_id2 = "red";
    Node *nodes2[] = {nodeA, nodeD};
    int times2[] = {30 * MINUTES};
    size_t route_size2 = 2;

    // Define trip attributes
    const char *trip_ids2[] = {"red-1", "red-2", "red-3",
                               "red-4", "red-5", "red-6"};
    int departures2[] = {9 * HOURS + 30 * MINUTES, 12 * HOURS + 30 * MINUTES,
                         18 * HOURS + 30 * MINUTES};
    Vehicle *vehicles2[] = {v6, v7, v8};
    size_t trip_size2 = 3;

    // Create route
    new_route(
        network, route_id2, nodes2, times2, route_size2,  // Route properties
        trip_ids2, departures2, vehicles2, trip_size2   // Trip properties
    );

    // Print network
    print_network(network);

    printf("\nTEST: ROUTING\n");
    // Route
    Connection *conn_1 = new_connection(nodeA, nodeD, 12 * HOURS);
    print_connection(conn_1);

    // One result
    Connection *conn_2 =
        new_connection(nodeA, nodeD, 18 * HOURS + 30 * MINUTES);
    print_connection(conn_2);

    // No result: Time
    Connection *conn_3 = new_connection(nodeA, nodeD, 24 * HOURS);
    print_connection(conn_3);

    // No result: No direct route
    Connection *conn_4 = new_connection(nodeA, nodeE, 24 * HOURS);
    print_connection(conn_4);

    // No result: Same place
    Connection *conn_5 = new_connection(nodeA, nodeA, 0 * HOURS);
    print_connection(conn_5);

    // Reservation
    printf("\nTEST: RESERVATION\n");
    print_connection(conn_1);
    printf("Reserve 9: %s\n",
           new_reservation(conn_1, 9) ? "success" : "failure");
    printf("Reserve 2: %s\n",
           new_reservation(conn_1, 2) ? "success" : "failure");
    printf("Reserve 2: %s\n",
           new_reservation(conn_2, 2) ? "success" : "failure");

    // Free memory
    printf("\nTEST: FREE MEMORY\n");
    delete_connection(conn_1->next->next);
    delete_connection(conn_2);
    delete_connection(conn_3);
    delete_connection(conn_4);
    delete_connection(conn_5);
    delete_network(network);

    return 0;
}