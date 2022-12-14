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
#include <osurs/reserve.h>

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

    Route *route = get_route(network, "ic-1-ew");
    print_route(route, 0);
    Trip *trip = get_trip(route, "ic-1-ew-2");
    print_trip(trip, 0);
    SeatCollection *sc = optimize_trip(trip);
    print_network(network);

    delete_network(network);

    return 0;
}
