/**
 * @brief Input and output of networks and its structures
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

#include "osurs/network.h"
#include "osurs/reserve.h"

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
 * @brief Import reservations from xml.
 *
 * @param network The network to import the reservations content to.
 * @param filename The file to import.
 * @return 1 if success, 0 if failure.
 */
int import_reservations(Network *network, const char *filename);

// io/export

/**
 * @brief Export a network to xml.
 *
 * @param network The network to export.
 * @param filename The file to create.
 * @return 1 if success, 0 if failure.
 */
int export_network(Network *network, const char *filename);

/**
 * @brief Export reservations of a network to xml.
 *
 * @param network The network to export the reservations.
 * @param filename The file to create.
 * @return 1 if success, 0 if failure.
 */
int export_reservations(Network *network, const char *filename);

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
 * @param indent The indent for printing.
 */
void print_node(Node *node, int indent);

/**
 * @brief Print composition.
 *
 * @param composition The composition struct to print.
 * @param indent The indent for printing.
 */
void print_composition(Composition *composition, int indent);

/**
 * @brief Print vehicle.
 *
 * @param vehicle The vehicle struct to print.
 * @param indent The indent for printing.
 */
void print_vehicle(Vehicle *vehicle, int indent);

/**
 * @brief Print stop.
 *
 * @param stop The stop struct to print.
 * @param indent The indent for printing.
 */
void print_stop(Stop *stop, int indent);

/**
 * @brief Print trip.
 *
 * @param trip The trip struct to print.
 * @param indent The indent for printing.
 */
void print_trip(Trip *trip, int indent);

/**
 * @brief Print route.
 *
 * @param route The route struct to print.
 * @param indent The indent for printing.
 */
void print_route(Route *route, int indent);

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
 * @param indent The indent for printing.
 */
void print_reservation(Reservation *reservation, int indent);

/**
 * @brief Print seat.
 *
 * @param seat The seat struct to print.
 * @param indent The indent for printing.
 */
void print_seat(Seat *seat, int indent);

/**
 * @brief Print seat collection.
 *
 * @param collection The seat collection struct to print.
 * @param indent The indent for printing.
 */
void print_seat_collection(SeatCollection *collection, int indent);

#endif  // OSURS_IO_H_