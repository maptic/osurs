/**
 * @brief The osurs command line interface
 *
 * (tbd.)
 *
 * @file main.c
 * @date: 2022-07-19
 * @author: Merlin Unterfinger
 */

#include <osurs/io.h>
#include <osurs/olal.h>
#include <stdio.h>

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
        // search direct connection between random nodes
        Node *orig = network->nodes[rand() % (network->node_counter)];
        Node *dest = network->nodes[rand() % (network->node_counter)];
        int dep = rand() % (24 * 60 * 60) + 1;
        Connection *conn = new_connection(orig, dest, dep);
        // book reservation if enough seats are available for connection
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

    // optimize a random trip
    Route *route = network->routes[rand() % (network->route_counter)];
    SeatCollection *collection = optimize_trip(route->root_trip);
    print_seat_collection(collection, 0);

    // plot reservations
    for (int i = 0; i < collection->seat_count; ++i) {
        Seat *seat = collection->seat_arr[i];
        for (int r = 0; r < seat->res_count; ++r) {
            printf("-%d-", seat->res_id_arr[r]);
        }
        if (seat->res_count > 0) {
            printf("\n");
        }
    }

    // cleanup
    delete_seat_collection(collection);
    delete_network(network);

    return 0;
}
