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
#include <string.h>
#include <unistd.h>

// io/import

/**
 * @brief Import a network to xml.
 *
 * @param network The empty network to import the content to.
 * @param filename The file to import.
 * @return 1 if success, 0 if failure.
 */
int import_network(Network *network, const char *filename);

/**
 * @brief Prints the current working dirctory.
 *
 * @return 1 if success, 0 if failure.
 */
int print_cwd();

/**
 * @brief Function print file content to the console.
 *
 * @param filename The path to the file.
 * @return 1 if success, 0 if failure.
 */
int print_file(const char *filename);

// io/export

/**
 * @brief Export a network to xml.
 *
 * @param network The network to export.
 * @param filename The file to create.
 * @return 1 if success, 0 if failure.
 */
int export_network(Network *network, const char *filename);

// io/matsim

/**
 * @brief
 *
 * @param network
 * @param schedule_file
 * @param vehicle_file
 * @return int
 */
int import_matsim(Network *network, const char *schedule_file,
                  const char *vehicle_file);

// io/print

/**
 * @brief Print node.
 *
 * @param node The node struct to print.
 */
void print_node(Node *node);

/**
 * @brief Print composition.
 *
 * @param composition The composition struct to print.
 */
void print_composition(Composition *composition);

/**
 * @brief Print vehicle.
 *
 * @param vehicle The vehicle struct to print.
 */
void print_vehicle(Vehicle *vehicle);

/**
 * @brief Print stop.
 *
 * @param stop The stop struct to print.
 */
void print_stop(Stop *stop);

/**
 * @brief Print trip.
 *
 * @param trip The trip struct to print.
 */
void print_trip(Trip *trip);

/**
 * @brief Print route.
 *
 * @param route The route struct to print.
 */
void print_route(Route *route);

/**
 * @brief Print network.
 *
 * @param network The network struct to print.
 */
void print_network(Network *network);

/**
 * @brief Print connection.
 *
 * @param connection The connection struct to print.
 */
void print_connection(Connection *connection);

/**
 * @brief Print reservation.
 *
 * @param reservation The reservation struct to print.
 */
void print_reservation(Reservation *reservation);

#endif  // OSURS_IO_H_