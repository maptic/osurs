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
    printf("<composition id=\"%s\" seats=\"%d\" />\n", composition->id,
           composition->seats);
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
    printf("<trip id=\"%s\" dep=\"%s\" arr=\"%s\" vid=\"%s\" />\n", trip->id,
           departure, arrival, trip->vehicle->id);
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

        printf("    <route id=\"%s\" >\n", route->id);
        printf("      <stops>\n");
        while (curr_stop != NULL) {
            printf("        ");
            print_stop(curr_stop);
            curr_stop = curr_stop->next;
        }
        printf("      </stops>\n");
        printf("      <trips>\n");
        while (curr_trip != NULL) {
            printf("        ");
            print_trip(curr_trip);
            curr_trip = curr_trip->next;
        }
        printf("      </trips>\n");
        printf("    </route>\n");
    }
    printf("  </routes>\n");
    printf("</network>\n");
}

void print_connection(Connection *connection) {
    Connection *curr_connection = connection;
    char arr[9];
    char dep[9];
    if (connection == NULL) {
        printf("No connection found.\n");
        return;
    }
    printf("<connection orig=\"%s\" dest=\"%s\">\n", connection->orig->node->id,
           connection->dest->node->id);
    while (curr_connection != NULL) {
        compose_time(dep, curr_connection->departure);
        compose_time(arr, curr_connection->arrival);
        printf(
            "  <alternative rid=\"%s\" tid=\"%s\" dep=\"%s\" arr=\"%s\" "
            "available=\"%d\" />\n",
            curr_connection->trip->route->id, curr_connection->trip->id, dep,
            arr, curr_connection->available);
        curr_connection = curr_connection->next;
    }
    printf("</connection>\n");
}

void print_reservation(Reservation *reservation) {
    printf("Reservation<tbd.>\n");
}
