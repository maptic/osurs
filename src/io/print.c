/**
 * @brief Input and output of networks and its structures.
 * @file print.c
 * @date: 2022-07-12
 * @author: Merlin Unterfinger
 */

#include <stdio.h>

#include "osurs/io.h"
#include "utils.h"

#define INDENT_CHARS ""
#define INDENT_DEPTH 4

void print_node(Node *node, int indent) {
    printf("%*s<node id=\"%s\" x=\"%.3f\" y=\"%.3f\" routes=\"%ld\" />\n",
           indent, INDENT_CHARS, node->id, node->x, node->y,
           node->route_counter);
}

void print_composition(Composition *composition, int indent) {
    printf("%*s<composition[id=\"%s\" seat_count=\"%d\" />\n", indent,
           INDENT_CHARS, composition->id, composition->seat_count);
}

void print_vehicle(Vehicle *vehicle, int indent) {
    printf("%*s<vehicle <id=\"%s\" cid=\"%s\" />\n", indent, INDENT_CHARS,
           vehicle->id, vehicle->composition->id);
}

void print_reservation(Reservation *reservation, int indent) {
    printf("%*s<reservation orig_nid=\"%s\" dest_nid=\"%s\" seats=\"%d\" />\n",
           indent, INDENT_CHARS, reservation->orig->node->id,
           reservation->dest->node->id, reservation->seats);
}

void print_seat(Seat *seat, int indent) {
    printf("%*s<seat sid=\"%d\" reservations=\"%d\" />\n", indent,
           INDENT_CHARS, seat->seat_id, seat->res_count);
}

void print_stop(Stop *stop, int indent) {
    char arrival_offset[9];
    char departure_offset[9];
    compose_time(arrival_offset, stop->arrival_offset);
    compose_time(departure_offset, stop->departure_offset);
    printf("%*s<stop arr_off=\"%s\" dep_off=\"%s\" nid=\"%s\" />\n", indent,
           INDENT_CHARS, arrival_offset, departure_offset, stop->node->id);
}

void print_trip(Trip *trip, int indent) {
    char departure[9];
    char arrival[9];
    compose_time(departure, trip->departure);
    compose_time(arrival, trip->arrival);
    printf("%*s<trip id=\"%s\" dep=\"%s\" arr=\"%s\" vid=\"%s\" res=\"%ld\"",
           indent, INDENT_CHARS, trip->id, departure, arrival,
           trip->vehicle->id, trip->reservation_counter);
    if (trip->reservation_counter > 0) {
        printf("\n");
        for (int i = 0; i < trip->reservation_counter; ++i) {
            print_reservation(trip->reservations[i], indent + INDENT_DEPTH);
        }
        printf("%*s</trip>\n", indent, INDENT_CHARS);
    } else {
        printf(" />\n");
    }
}

void print_route(Route *route, int indent) {
    Stop *curr_stop = route->root_stop;
    Trip *curr_trip = route->root_trip;
    printf("%*s<route id=\"%s\">\n", indent, INDENT_CHARS, route->id);
    printf("%*s<stops>\n", indent + INDENT_DEPTH, INDENT_CHARS);
    while (curr_stop != NULL) {
        print_stop(curr_stop, indent + 2 * INDENT_DEPTH);
        curr_stop = curr_stop->next;
    }
    printf("%*s</stops>\n", indent + INDENT_DEPTH, INDENT_CHARS);
    printf("%*s<trips>\n", indent + INDENT_DEPTH, INDENT_CHARS);
    while (curr_trip != NULL) {
        print_trip(curr_trip, indent + 2 * INDENT_DEPTH);
        curr_trip = curr_trip->next;
    }
    printf("%*s</trips>\n", indent + INDENT_DEPTH, INDENT_CHARS);
    printf("%*s</route>\n", indent, INDENT_CHARS);
}

void print_network(Network *network) {
    int indent = 0;
    // Network
    printf(
        "<network nodes=\"%ld\" composition=\"%ld\" vehicles=\"%ld\" "
        "routes=\"%ld\" >\n",
        network->node_counter, network->composition_counter,
        network->vehicle_counter, network->route_counter);
    // Nodes
    printf("%*s<nodes>\n", indent + INDENT_DEPTH, INDENT_CHARS);
    for (size_t i = 0; i < network->node_counter; ++i) {
        print_node(network->nodes[i], indent + 2 * INDENT_DEPTH);
    }
    printf("%*s</nodes>\n", indent + INDENT_DEPTH, INDENT_CHARS);
    // Compositions
    printf("%*s<compositions>\n", indent + INDENT_DEPTH, INDENT_CHARS);
    for (size_t i = 0; i < network->composition_counter; ++i) {
        print_composition(network->compositions[i], indent + 2 * INDENT_DEPTH);
    }
    printf("%*s</compositions>\n", indent + INDENT_DEPTH, INDENT_CHARS);
    // Vehicles
    printf("%*s<vehicles>\n", indent + INDENT_DEPTH, INDENT_CHARS);
    for (size_t i = 0; i < network->vehicle_counter; ++i) {
        print_vehicle(network->vehicles[i], indent + 2 * INDENT_DEPTH);
    }
    printf("%*s</vehicles>\n", indent + INDENT_DEPTH, INDENT_CHARS);
    // Routes
    printf("%*s<routes>\n", indent + INDENT_DEPTH, INDENT_CHARS);
    for (size_t i = 0; i < network->route_counter; ++i) {
        Route *route = network->routes[i];
        Stop *curr_stop = route->root_stop;
        Trip *curr_trip = route->root_trip;
        print_route(route, indent + 2 * INDENT_DEPTH);
    }
    printf("%*s</routes>\n", indent + INDENT_DEPTH, INDENT_CHARS);
    printf("</network>\n");
}

void print_connection(Connection *connection) {
    Connection *curr_conn = connection;
    Connection *set_conn = connection;
    char *element_type;
    char arr[9];
    char dep[9];

    // Check if any.
    if (connection == NULL) {
        printf("No connection found.\n");
        return;
    }

    // Iterate to root / head of chain.
    while (curr_conn->prev != NULL) {
        curr_conn = curr_conn->prev;
    }

    // Print connection
    printf("<connection orig=\"%s\" dest=\"%s\">\n", connection->orig->node->id,
           connection->dest->node->id);
    while (curr_conn != NULL) {
        element_type = "alternative";
        compose_time(dep, curr_conn->departure);
        compose_time(arr, curr_conn->arrival);
        if (curr_conn == set_conn) {
            element_type = "selected";
        }
        printf(
            "  <%s rid=\"%s\" tid=\"%s\" dep=\"%s\" arr=\"%s\" "
            "available=\"%d\" />\n",
            element_type, curr_conn->trip->route->id, curr_conn->trip->id, dep,
            arr, curr_conn->available);
        curr_conn = curr_conn->next;
    }
    printf("</connection>\n");
}

void print_seat_collection(SeatCollection *collection, int indent) {
    printf("%*s<collection seats=\"%d\">\n", indent, INDENT_CHARS,
           collection->seat_count);
    for (int i = 0; i < collection->seat_count; ++i) {
        print_seat(collection->seat_arr[i], indent + INDENT_DEPTH);
    }
    printf("%*s</collection>\n", indent, INDENT_CHARS);
}