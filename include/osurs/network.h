/**
 * @brief Network for reservation optmization
 *
 * The network consists of nodes with a name and coordinates. A route is defined
 * by stops and trips. Each stop is associated to a node of the network and
 * knows the previous and next stop if there is one. A route can have several
 * trips, which are defined by departure times and their seat capacity. Each
 * trip knows the next departing trip, if there is one.
 *
 * @file network.h
 * @date: 2022-07-12
 * @author: Merlin Unterfinger
 */

#ifndef OSURS_NETWORK_H_
#define OSURS_NETWORK_H_

#include <osurs/types.h>

#define INIT_ALLOC_SIZE_S 10
#define INIT_ALLOC_SIZE_M 100
#define INIT_ALLOC_SIZE_L 1000

// Constructor-like methods

/**
 * @brief Create a new nework
 *
 * The network consists of nodes where vehicles stop and passengers can get on
 * and off. A route stores the order in which the nodes are approached by a
 * vehicle in a chain of stops. Each stop contains information about which stop
 * is next and how long it takes to reach it. On routes, trips indicate the
 * departure times at which a vehicle leaves from the route's root stop. Vehicle
 * information such as capacity and reservations are stored at the trip level.
 *
 * @note All objects of the network are located on the heap and are directly or
 * indirectly linked to the network structure. If the memory of the network is
 * released (delete_network(Network *)) all associated structures of the network
 * are also cleared.
 *
 * @return Network*
 */
Network *new_network();

/**
 * @brief Create a new node and add it to the network.
 *
 * @param network The network to add the node.
 * @param id The id of the node.
 * @param x The x coordinate of the node.
 * @param y The y coordinate of the node.
 * @return A pointer to the newly allocated node (Node*).
 */
Node *new_node(Network *network, const char *id, double x, double y);

/**
 * @brief Create a new composition and add it to the network.
 *
 * @param network The network to add the composition.
 * @param id The id of the composition.
 * @param seats The seat capacity of the composition.
 * @return A pointer to the newly allocated node (Composition*).
 */
Composition *new_composition(Network *network, const char *id, int seats);

/**
 * @brief Create a new vehicle and add it to the network.
 *
 * @param network The network to add the vehicle.
 * @param id The id of the vehicle.
 * @param composition The composition the vehicle has.
 * @return A pointer to the newly allocated vehicle (Vehicle*).
 */
Vehicle *new_vehicle(Network *network, const char *id,
                     Composition *composition);

/**
 * @brief Create a new route and add it to the network.
 *
 * @param network The network to add the route.
 * @param id The identifier of the route.
 * @param nodes A pointer to an array of nodes, which define the stops of the
 * route (n=route_size).
 * @param arrival_offsets A pointer to an array of arrival offset times from the
 * root stop. The offsets define the travel times between the departure of the
 * root stop and the arrival at the current stop (n=route_size).
 * @param departure_offsets A pointer to an array of departure offset times from
 * the root stop. The offsets define the travel times between the departure of
 * the root stop and the departure at the current stop (n=route_size).
 * @param route_size The number of nodes (=stops) in the route.
 * @param trip_ids The identifier of the trips on the route (n=trip_size).
 * @param departures The departure times at the first node (=root_stop) of the
 * route in seconds after midnight (00:00:00). Every departure time will define
 * a new trip, which starts traveling along the route (n=trip_size)
 * @param vehicles An array of vehicles for each trip (n=trip_size).
 * @param trip_size The number of departures (=trips) on the route.
 * @return A pointer to the newly allocated node (Route*).
 */
Route *new_route(Network *network, const char *id, Node *nodes[],
                 int arrival_offsets[], int departure_offsets[],
                 size_t route_size, const char *trip_ids[], int departures[],
                 Vehicle *vehicles[], size_t trip_size);

// Getters

/**
 * @brief Get the node struct.
 *
 * @param network An network to get the node from.
 * @param id The identifier of the node.
 * @return Returns the node.
 */
Node *get_node(Network *network, const char *id);

/**
 * @brief Get the vehicle struct.
 *
 * @param network An network to get the vehicle from.
 * @param id The identifier of the vehicle.
 * @return Returns the vehicle.
 */
Vehicle *get_vehicle(Network *network, const char *id);

/**
 * @brief Get the composition struct.
 *
 * @param network An network to get the composition from.
 * @param id The identifier of the composition.
 * @return Returns the composition.
 */
Composition *get_composition(Network *network, const char *id);

/**
 * @brief Get the route struct.
 *
 * @param network An network to get the route from.
 * @param id The identifier of the route.
 * @return Returns the route.
 */
Route *get_route(Network *network, const char *id);

// Destructor-like methods

/**
 * @brief Delete a network.
 *
 * All objects of the network are located on the heap and are directly or
 * indirectly linked to the network structure. If the memory of the network is
 * released all associated structures of the network are also cleared.
 *
 * @param network The network to delete and free.
 */
void delete_network(Network *network);

#endif  // OSURS_NETWORK_H_