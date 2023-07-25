/**
 * @brief A seat optimization example
 *
 * Compile:
 *  gcc olal_example.c -o olal_example -losurs-io -losurs-network -losurs-ds -losurs-reserve -losurs-optimize -losurs-olal -lxml2
 *
 * @file olal_example.c
 * @date: 2023-02-22
 * @author: Merlin Unterfinger
 */

#include <osurs/io.h>
#include <osurs/olal.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    // read network
    Network *network = new_network();
    if (!import_network(network, "../tests/input/intercity_network.xml")) {
        perror("Could not load network");
        return 1;
    }

    // create random reservations
    for (int i = 0; i < 1000000; ++i) {
        // search direct connection between random nodes
        Node *orig = hash_map_get_random(network->nodes);
        Node *dest = hash_map_get_random(network->nodes);
        int dep = rand() % (24 * 60 * 60) + 1;
        Connection *conn = new_connection(orig, dest, dep);
        // book reservation if enough seats are available for connection
        int seats = rand() % 3 + 1;
        Reservation *res = new_reservation(conn, seats, NULL);
        delete_connection(conn);
    }

    // optimize a random trip
    Route *route = hash_map_get_random(network->routes);
    SeatCollection *collection = optimize_trip(route->root_trip);
    print_seat_collection(collection, 0);

    // cleanup
    delete_seat_collection(collection);
    delete_network(network);

    return 0;
}