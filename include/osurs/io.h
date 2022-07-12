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

#include <osurs/network.h>
#include <stdio.h>

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

#endif  // OSURS_IO_H_