/**
 * @brief Input and output of networks and its strutures
 * @file print.c
 * @date: 2022-07-12
 * @author: Merlin Unterfinger
 */

#include <osurs/io.h>

#include "utils.h"

void print_node(Node *node) {
    printf("<node id=\"%s\" x=\"%.3f\" y=\"%.3f\" routes=\"%ld\" />\n",
           node->id, node->x, node->y, node->route_counter);
}

void print_composition(Composition *composition) {
    printf("<composition id=\"%s\" seat_count=\"%d\" />\n", composition->id,
           composition->seat_count);
}

void print_vehicle(Vehicle *vehicle) {
    printf("<vehicle id=\"%s\" cid=\"%s\" />\n", vehicle->id,
           vehicle->composition->id);
}

void print_stop(Stop *stop) {
    char arrival_offset[9];
    char departure_offset[9];
    compose_time(arrival_offset, stop->arrival_offset);
    compose_time(departure_offset, stop->departure_offset);
    printf("<stop arr_off=\"%s\" dep_off=\"%s\" nid=\"%s\" />\n",
           arrival_offset, departure_offset, stop->node->id);
}

void print_trip(Trip *trip) {
    char departure[9];
    char arrival[9];
    compose_time(departure, trip->departure);
    compose_time(arrival, trip->arrival);
    printf("<trip id=\"%s\" dep=\"%s\" arr=\"%s\" vid=\"%s\" res=\"%ld\"",
           trip->id, departure, arrival, trip->vehicle->id,
           trip->reservation_counter);
    if (trip->reservation_counter > 0) {
        printf("\n");
        for (int i = 0; i < trip->reservation_counter; ++i) {
            printf("  ");
            print_reservation(trip->reservations[i]);
        }
        printf("\n</trip>\n");
    } else {
        printf(" />\n");
    }
}

void print_route(Route *route) {
    Stop *curr_stop = route->root_stop;
    Trip *curr_trip = route->root_trip;

    printf("<route id=\"%s\">\n", route->id);
    printf("\t<stops>\n");
    while (curr_stop != NULL) {
        printf("\t\t");
        print_stop(curr_stop);
        curr_stop = curr_stop->next;
    }
    printf("\t</stops>\n");
    printf("\t<trips>\n");
    while (curr_trip != NULL) {
        printf("\t\t");
        print_trip(curr_trip);
        curr_trip = curr_trip->next;
    }
    printf("\t</trips>\n");
    printf("</route>\n");
}

void print_network(Network *network) {
    // Network
    printf(
        "<network nodes=\"%ld\" composition=\"%ld\" vehicles=\"%ld\" "
        "routes=\"%ld\" >\n",
        network->node_counter, network->composition_counter,
        network->vehicle_counter, network->route_counter);
    // Nodes
    printf("  <nodes>\n");
    for (size_t i = 0; i < network->node_counter; ++i) {
        printf("    ");
        print_node(network->nodes[i]);
    }
    printf("  </nodes>\n");
    // Compostitions
    printf("  <compositions>\n");
    for (size_t i = 0; i < network->composition_counter; ++i) {
        printf("    ");
        print_composition(network->compositions[i]);
    }
    printf("  </compositions>\n");
    // Vehicles
    printf("  <vehicles>\n");
    for (size_t i = 0; i < network->vehicle_counter; ++i) {
        printf("    ");
        print_vehicle(network->vehicles[i]);
    }
    printf("  </vehicles>\n");
    // Routes
    printf("  <routes>\n");
    for (size_t i = 0; i < network->route_counter; ++i) {
        Route *route = network->routes[i];
        Stop *curr_stop = route->root_stop;
        Trip *curr_trip = route->root_trip;
        // Route
        printf("    <route id=\"%s\" >\n", route->id);
        // Stops
        printf("      <stops>\n");
        while (curr_stop != NULL) {
            printf("        ");
            print_stop(curr_stop);
            curr_stop = curr_stop->next;
        }
        printf("      </stops>\n");
        // Trips
        printf("      <trips>\n");
        while (curr_trip != NULL) {
            // Trip
            char departure[9];
            char arrival[9];
            compose_time(departure, curr_trip->departure);
            compose_time(arrival, curr_trip->arrival);
            printf(
                "        <trip id=\"%s\" dep=\"%s\" arr=\"%s\" vid=\"%s\" "
                "res=\"%ld\"",
                curr_trip->id, departure, arrival, curr_trip->vehicle->id,
                curr_trip->reservation_counter);
            if (curr_trip->reservation_counter > 0) {
                printf(">\n");
                for (int i = 0; i < curr_trip->reservation_counter; ++i) {
                    printf("          ");
                    print_reservation(curr_trip->reservations[i]);
                }
                printf("        </trip>\n");
            } else {
                printf(" />\n");
            }

            curr_trip = curr_trip->next;
        }
        printf("      </trips>\n");
        printf("    </route>\n");
    }
    printf("  </routes>\n");
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

void print_reservation(Reservation *reservation) {
    printf("<reservation orig_nid=\"%s\" dest_nid=\"%s\" seats=\"%d\" />\n",
           reservation->orig->node->id, reservation->dest->node->id,
           reservation->seats);
}