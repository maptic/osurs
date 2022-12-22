/**
 * @brief Command line interface for converting transit schedules and printing
 * networks.
 *
 * This file contains functions for converting transit schedules from the Matsim
 * format to the osurs format, and for printing the network from a file to
 * stdout. The file also contains the main function for the command line
 * interface, which allows the user to specify commands and arguments to invoke
 * these functions.
 *
 * @file cli.c
 * @date 2022-12-22
 * @author Merlin Unterfinger
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "osurs/io.h"

/**
 * @brief Convert a transit schedule from the Matsim format to the osurs format.
 *
 * @param schedule_file The Matsim schedule file to convert.
 * @param vehicle_file The Matsim vehicle file to convert.
 * @param network_file The output osurs network file.
 * @return 1 if success, 0 if failure.
 */
int convert_schedule(const char *schedule_file, const char *vehicle_file,
                     const char *network_file) {
    // Create a new network
    Network *network = new_network();
    if (!network) {
        fprintf(stderr, "Error: Unable to create network.\n");
        return 0;
    }

    // Import the schedule and vehicle data from the Matsim files
    if (!import_matsim(network, schedule_file, vehicle_file)) {
        fprintf(stderr, "Error: Unable to import Matsim data.\n");
        delete_network(network);
        return 0;
    }

    // Export the network data to the osurs network file
    if (!export_network(network, network_file)) {
        fprintf(stderr, "Error: Unable to export network data.\n");
        delete_network(network);
        return 0;
    }

    // Clean up and return success
    delete_network(network);
    return 1;
}

/**
 * @brief Print the network from a file to stdout.
 *
 * @param network_file The osurs network file to print.
 * @param reservations_file The osurs reservations file to print.
 * @return 1 if success, 0 if failure.
 */
int print_network_from_file(const char *network_file,
                            const char *reservations_file) {
    // Create a new network
    Network *network = new_network();
    if (!network) {
        fprintf(stderr, "Error: Unable to create network.\n");
        return 0;
    }

    // Import the network and reservations data from the files
    if (!import_network(network, network_file)) {
        fprintf(stderr, "Error: Unable to import network data.\n");
        delete_network(network);
        return 0;
    }
    if (reservations_file != NULL) {
        if (!import_reservations(network, reservations_file)) {
            fprintf(stderr, "Error: Unable to import reservations data.\n");
            delete_network(network);
            return 0;
        }
    }

    // Print the network to stdout
    print_network(network);

    // Clean up
    delete_network(network);
    return 1;
}

/**
 * @brief Main function for the command line interface.
 *
 * Usage: cli [command] [arguments]
 *
 * Available commands:
 *   convert [matsim_schedule_file] [matsim_vehicle_file] [osurs_network_file]
 *   print [osurs_network_file] [optional osurs_reservations_file]
 *
 * @param argc Number of arguments.
 * @param argv Array of arguments.
 * @return 0 if success, non-zero if failure.
 */
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Error: No command specified.\n");
        return 1;
    }
    // Print help message if "--help" flag is present
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        printf("Usage: %s [command] [arguments]\n", argv[0]);
        printf("Available commands:\n");
        printf("  %s\n  %s\n",
               "convert [matsim_schedule_file] [matsim_vehicle_file] "
               "[osurs_network_file]",
               "print [osurs_network_file] [optional osurs_reservations_file]");
        return 0;
    }
    // Convert command
    if (strcmp(argv[1], "convert") == 0) {
        if (argc != 5) {
            fprintf(
                stderr,
                "Error: Invalid number of arguments for convert command.\n");
            return 1;
        }
        if (!convert_schedule(argv[2], argv[3], argv[4])) {
            return 1;
        }
    }
    // Print command
    else if (strcmp(argv[1], "print") == 0) {
        if (argc != 3 && argc != 4) {
            fprintf(stderr,
                    "Error: Invalid number of arguments for print command.\n");
            return 1;
        }
        char *reservation_file = NULL;
        if (argc == 4) {
            reservation_file = argv[3];
        }
        print_network_from_file(argv[2], reservation_file);
    }
    // Invalid command
    else {
        fprintf(stderr, "Error: Invalid command.\n");
        return 1;
    }

    return 0;
}
