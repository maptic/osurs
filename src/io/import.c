/**
 * @brief Import a network file.
 * @file import.c
 * @date: 2022-10-01
 * @author: Merlin Unterfinger
 */

#include <libxml/parser.h>
#include <limits.h>
#include <stdio.h>

#include "osurs/io.h"
#include "utils.h"

// Private declarations

typedef struct carrier_t Carrier;
static Carrier *new_carrier(Network *network);
static void empty_carrier(Carrier *carrier);
static Carrier *init_carrier(Carrier *carrier, char *route_id,
                             size_t route_size, size_t trip_size);
static void new_route_from_carrier(Carrier *carrier);
static void delete_carrier(Carrier *carrier);

static void network_parser(xmlNode *xml_node, Carrier *carrier);
static void reservation_parser(xmlNode *xml_node, Network *network,
                               Route *route, Trip *trip);
static void handle_node(xmlNode *xml_node, Network *network);
static void handle_composition(xmlNode *xml_node, Network *network);
static void handle_vehicle(xmlNode *xml_node, Network *network);
static void handle_route(xmlNode *xml_node, Carrier *carrier);
static void handle_stop(xmlNode *xml_node, Carrier *carrier);
static void handle_trip(xmlNode *xml_node, Carrier *carrier);

// Public definitions

int import_network(Network *network, const char *filename) {
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;
    Carrier *carrier = new_carrier(network);

    // Parse transit vehicles file
    doc = xmlReadFile(filename, NULL, 0);
    if (doc == NULL) {
        printf("Could not parse file %s.", filename);
        return 0;
    }
    root_element = xmlDocGetRootElement(doc);
    network_parser(root_element, carrier);

    // Add last route
    new_route_from_carrier(carrier);
    delete_carrier(carrier);

    // Clean up
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return 1;
}

int import_reservations(Network *network, const char *filename) {
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    // Parse transit vehicles file
    doc = xmlReadFile(filename, NULL, 0);
    if (doc == NULL) {
        printf("Could not parse file %s.", filename);
        return 0;
    }
    root_element = xmlDocGetRootElement(doc);
    reservation_parser(root_element, network, NULL, NULL);

    // Clean up
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return 1;
}

// Private definitions

typedef struct carrier_t {
    Network *network;
    char *route_id;
    Node **nodes;
    int *arrival_offsets;
    int *departure_offsets;
    size_t stop_count;
    size_t route_size;
    const char **trip_ids;
    int *departures;
    struct vehicle_t **vehicles;
    size_t trip_count;
    size_t trip_size;
    int init_state;
} Carrier;

static Carrier *new_carrier(Network *network) {
    Carrier *carrier = (Carrier *)malloc(sizeof(Carrier));
    carrier->network = network;
    carrier->init_state = 0;
    return carrier;
}

static void empty_carrier(Carrier *carrier) {
    xmlFree(carrier->route_id);
    free(carrier->nodes);
    free(carrier->arrival_offsets);
    free(carrier->departure_offsets);
    for (int i = 0; i < carrier->trip_size; i++)
        xmlFree((char *)carrier->trip_ids[i]);
    free(carrier->trip_ids);
    free(carrier->departures);
    free(carrier->vehicles);
}

static Carrier *init_carrier(Carrier *carrier, char *route_id,
                             size_t route_size, size_t trip_size) {
    carrier->route_id = route_id;
    carrier->nodes = (Node **)malloc(sizeof(Node *) * route_size);
    carrier->arrival_offsets = (int *)malloc(sizeof(int) * route_size);
    carrier->departure_offsets = (int *)malloc(sizeof(int) * route_size);
    carrier->route_size = route_size;
    carrier->stop_count = 0;
    carrier->trip_ids = (const char **)malloc(sizeof(char *) * trip_size);
    carrier->departures = (int *)malloc(sizeof(int) * trip_size);
    carrier->vehicles = (Vehicle **)malloc(sizeof(Vehicle) * trip_size);
    carrier->trip_size = trip_size;
    carrier->trip_count = 0;
    carrier->init_state = 1;
    return carrier;
}

static void new_route_from_carrier(Carrier *carrier) {
    new_route(carrier->network, carrier->route_id, carrier->nodes,
              carrier->arrival_offsets, carrier->departure_offsets,
              carrier->route_size, carrier->trip_ids, carrier->departures,
              carrier->vehicles, carrier->trip_size);
}

static void delete_carrier(Carrier *carrier) {
    empty_carrier(carrier);
    free(carrier);
}

static void network_parser(xmlNode *xml_node, Carrier *carrier) {
    while (xml_node) {
        if (xml_node->type == XML_ELEMENT_NODE) {
            if (xmlStrcmp(xml_node->name, "node") == 0) {
                handle_node(xml_node, carrier->network);
            } else if (xmlStrcmp(xml_node->name, "composition") == 0) {
                handle_composition(xml_node, carrier->network);
            } else if (xmlStrcmp(xml_node->name, "vehicle") == 0) {
                handle_vehicle(xml_node, carrier->network);
            } else if (xmlStrcmp(xml_node->name, "route") == 0) {
                handle_route(xml_node, carrier);
            } else if (xmlStrcmp(xml_node->name, "stop") == 0) {
                handle_stop(xml_node, carrier);
            } else if (xmlStrcmp(xml_node->name, "trip") == 0) {
                handle_trip(xml_node, carrier);
            }
        }
        network_parser(xml_node->children, carrier);
        xml_node = xml_node->next;
    }
}

static void reservation_parser(xmlNode *xml_node, Network *network,
                               Route *route, Trip *trip) {
    while (xml_node) {
        if (xml_node->type == XML_ELEMENT_NODE) {
            if (xmlStrcmp(xml_node->name, "reservation") == 0) {
                char *id_tmp = xmlGetProp(xml_node, "id");
                char *rid_tmp = xmlGetProp(xml_node, "rid");
                char *tid_tmp = xmlGetProp(xml_node, "tid");
                char *nid_orig_tmp = xmlGetProp(xml_node, "nid_orig");
                char *nid_dest_tmp = xmlGetProp(xml_node, "nid_dest");
                char *seats_tmp = xmlGetProp(xml_node, "seat_count");

                // Get route
                if (route == NULL || xmlStrcmp(rid_tmp, route->id) != 0) {
                    route = get_route(network, rid_tmp);
                }

                // Get trip
                if (trip == NULL || xmlStrcmp(tid_tmp, trip->id) != 0) {
                    trip = route->root_trip;
                    while (trip) {
                        if (xmlStrcmp(tid_tmp, trip->id) == 0) break;
                        trip = trip->next;
                    }
                }

                // Get orig
                Stop *orig = route->root_stop;
                while (orig) {
                    if (xmlStrcmp(nid_orig_tmp, orig->node->id) == 0) break;
                    orig = orig->next;
                }

                // Get dest
                Stop *dest = orig;
                while (dest) {
                    if (xmlStrcmp(nid_dest_tmp, dest->node->id) == 0) break;
                    dest = dest->next;
                }

                // Recreate connection
                Connection *conn = (Connection *)malloc(sizeof(Connection));
                conn->prev = NULL;
                conn->next = NULL;
                conn->orig = orig;
                conn->dest = dest;
                conn->trip = trip;
                conn->departure = 0;
                conn->arrival = 1;
                conn->available = INT_MAX;

                // Create new reservation
                int seat_count;
                sscanf(seats_tmp, "%d", &seat_count);
                if (new_reservation(conn, seat_count, id_tmp) == NULL) {
                    printf(
                        "ERROR: Failed to create reservation (%s -> %s, "
                        "trip=%s, seat_count=%d).\n",
                        orig->node->id, dest->node->id, trip->id, seat_count);
                }

                // Free heap
                delete_connection(conn);
                xmlFree(id_tmp);
                xmlFree(rid_tmp);
                xmlFree(tid_tmp);
                xmlFree(nid_orig_tmp);
                xmlFree(nid_dest_tmp);
                xmlFree(seats_tmp);
            }
        }
        reservation_parser(xml_node->children, network, route, trip);
        xml_node = xml_node->next;
    }
}

static void handle_node(xmlNode *xml_node, Network *network) {
    double x, y;
    char *id_tmp = xmlGetProp(xml_node, "id");
    char *x_tmp = xmlGetProp(xml_node, "x");
    char *y_tmp = xmlGetProp(xml_node, "y");
    sscanf(x_tmp, "%lf", &x);
    sscanf(y_tmp, "%lf", &y);
    new_node(network, id_tmp, x, y);
    xmlFree(id_tmp);
    xmlFree(x_tmp);
    xmlFree(y_tmp);
}

static void handle_composition(xmlNode *xml_node, Network *network) {
    int seat_count;
    char *id_tmp = xmlGetProp(xml_node, "id");
    char *seat_count_tmp = xmlGetProp(xml_node, "seat_count");
    sscanf(seat_count_tmp, "%d", &seat_count);
    new_composition(network, id_tmp, seat_count);
    xmlFree(id_tmp);
    xmlFree(seat_count_tmp);
}

static void handle_vehicle(xmlNode *xml_node, Network *network) {
    char *id_tmp = xmlGetProp(xml_node, "id");
    char *cid_tmp = xmlGetProp(xml_node, "cid");
    Composition *composition = get_composition(network, cid_tmp);
    new_vehicle(network, id_tmp, composition);
    xmlFree(id_tmp);
    xmlFree(cid_tmp);
}

static void handle_route(xmlNode *xml_node, Carrier *carrier) {
    // Add Route if carrier already filled
    if (carrier->init_state == 1) {
        new_route_from_carrier(carrier);
        empty_carrier(carrier);
        carrier->init_state = 0;
    }
    // Get route id, stops and trips
    size_t route_size;
    size_t trip_size;
    char *route_id_tmp = xmlGetProp(xml_node, "id");
    char *stops_tmp = xmlGetProp(xml_node, "stops");
    char *trips_tmp = xmlGetProp(xml_node, "trips");
    sscanf(stops_tmp, "%ld", &route_size);
    sscanf(trips_tmp, "%ld", &trip_size);
    xmlFree(stops_tmp);
    xmlFree(trips_tmp);
    // Set carrier size
    init_carrier(carrier, route_id_tmp, route_size, trip_size);
}

static void handle_stop(xmlNode *xml_node, Carrier *carrier) {
    // Parse stop element
    char *nid_tmp = xmlGetProp(xml_node, "nid");
    char *arr_off_tmp = xmlGetProp(xml_node, "arr_off");
    char *dep_off_tmp = xmlGetProp(xml_node, "dep_off");
    int arr_off = parse_time(arr_off_tmp);
    int dep_off = parse_time(dep_off_tmp);
    Node *node = get_node(carrier->network, nid_tmp);
    xmlFree(nid_tmp);
    xmlFree(arr_off_tmp);
    xmlFree(dep_off_tmp);
    // Add stop
    carrier->nodes[carrier->stop_count] = node;
    carrier->arrival_offsets[carrier->stop_count] = arr_off;
    carrier->departure_offsets[carrier->stop_count] = dep_off;
    ++(carrier->stop_count);
}

static void handle_trip(xmlNode *xml_node, Carrier *carrier) {
    // Parse trip element
    char *tid_tmp = xmlGetProp(xml_node, "id");  // Delete afterwards in carrier
    char *dep_tmp = xmlGetProp(xml_node, "dep");
    char *vid_tmp = xmlGetProp(xml_node, "vid");
    int dep = parse_time(dep_tmp);
    Vehicle *vehicle = get_vehicle(carrier->network, vid_tmp);
    xmlFree(vid_tmp);
    xmlFree(dep_tmp);
    // Add trip
    carrier->trip_ids[carrier->trip_count] = tid_tmp;
    carrier->departures[carrier->trip_count] = dep;
    carrier->vehicles[carrier->trip_count] = vehicle;
    ++(carrier->trip_count);
}
