/**
 * @brief The osurs command line interface
 *
 * (tbd.)
 *
 * @file main.c
 * @date: 2022-07-19
 * @author: Merlin Unterfinger
 */

#include <stdio.h>
#include <osurs/io.h>
#include <osurs/olal.h>

int main(int argc, char *argv[]) {
    if (argc > 0) {
        // printf("usage: %s <arg>\n", argv[0]);
    }

    Network *network = new_network();
    if (!import_network(network, "../tests/input/intercity_network.xml")) {
        perror("Could not load network.");
        return 1;
    }
    if (!import_reservations(network,
                             "../tests/input/intercity_reservations.xml")) {
        perror("Could not load reservations.");
        return 1;
    }

    // Create random reservations
    int res_count = 0;
    int seat_count = 0;
    for (int i = 0; i < 1000000; ++i) {
        int idx_1 = rand() % (network->node_counter);
        int idx_2 = rand() % (network->node_counter);
        Node *orig = network->nodes[idx_1];
        Node *dest = network->nodes[idx_2];
        int dep = rand() % ((24 * 60 * 60 + 1) - 0) + 0;
        Connection *conn = new_connection(orig, dest, dep);
        int seats = rand() % 3 + 1;
        Reservation *res = new_reservation(conn, seats);
        if (res != NULL) {
            ++res_count;
            seat_count += seats;
        }
        delete_connection(conn);
    }
    printf("Generated %d reservations with %d occupied seats.\n", res_count,
           seat_count);

    // Optimize a random trip
    Route *route = network->routes[rand() % (network->route_counter)];
    SeatCollection *collection = optimize_trip(route->root_trip);
    print_seat_collection(collection, 0);

    // Get trip with maximum reservations
    // Route *route = get_route(network, "ic-1-ew");
    // print_route(route, 0);
    // Trip *trip = get_trip(route, "ic-1-ew-2");
    // print_trip(trip, 0);
    // SeatCollection *collection = optimize_trip(trip);
    // print_seat_collection(collection, 0);
    // print_network(network);
    // delete_network(network);

    return 0;
}
