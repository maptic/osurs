/**
 * @brief Input and output of networks and its strutures
 *
 * Printing osurs structures, importing and exporting networks, vehicles and
 * reservations.
 *
 * @file io.h
 * @date: 2022-07-12
 * @author: Merlin Unterfinger
 */

#ifndef OSURS_IO_H_
#define OSURS_IO_H_

#include <limits.h>
#include <osurs/network.h>
#include <stdio.h>
#include <unistd.h>

/**
 * @brief Print node.
 *
 * @param Node The node struct to print.
 */
void print_node(Node *node);

/**
 * @brief Print stop.
 *
 * @param Stop The stop struct to print.
 */
void print_stop(Stop *stop);

/**
 * @brief Print trip.
 *
 * @param Trip The trip struct to print.
 */
void print_trip(Trip *trip);

/**
 * @brief Print route.
 *
 * @param Route The route struct to print.
 */
void print_route(Route *route);

/**
 * @brief Print network.
 *
 * @param Network The network struct to print.
 */
void print_network(Network *network);

/**
 * @brief Print connection.
 *
 * @param Connection The connection struct to print.
 */
void print_connection(Connection *connection);

/**
 * @brief Print reservation.
 *
 * @param reservation The reservation struct to print.
 */
void print_reservation(Reservation *reservation);

/**
 * @brief Prints the current working dirctory.
 *
 * @return 0 if success, 1 if error.
 */
int print_cwd();

/**
 * @brief Function print file content to the console.
 *
 * @param filename The path to the file.
 * @return 0 if success, 1 if error.
 */
int print_file(const char *filename);

/**
 * @brief Import a MATSim transit schedule.
 *
 * Import a MATSim transit schedule from a transitSchedule.xml and  vehicles.xml
 * file.
 *
 * @param network Pointer to an empty network.
 * @param schedule_file File path to the transit schedule file.
 * @param vehicle_file File path to the vehicles file.
 * @return Returns 1 if success and 0 if failure.
 */
int import_matsim(Network *network, const char *schedule_file,
                  const char *vehicle_file);

#endif  // OSURS_IO_H_