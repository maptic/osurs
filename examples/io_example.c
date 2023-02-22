/**
 * @brief An IO example
 *
 * Compile:
 *  gcc io_example.c -o io_example -losurs-io -losurs-network -losurs-ds -losurs-reserve -lxml2
 *
 * @file io_example.c
 * @date: 2023-02-22
 * @author: Merlin Unterfinger
 */

#include <stdio.h>
#include <osurs/io.h>

int main(int argc, char* argv[]) {
    Network* network = new_network();
    if (!import_network(network, "../tests/input/intercity_network.xml")) {
        perror("Could not load network");
        return 1;
    }
    if (!import_reservations(network, "../tests/input/intercity_reservations.xml")) {
        perror("Could not load reservations");
        return 1;
    }
    print_network(network);
    delete_network(network);
    return 0;
}