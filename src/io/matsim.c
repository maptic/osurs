/**
 * @brief Import a MATSim transit schedule from XML.
 * @file matsim.c
 * @date: 2022-07-19
 * @author: Merlin Unterfinger
 */

#include <libxml/parser.h>

#include "osurs/io.h"
#include "utils.h"

#define INIT_ALLOC_SIZE_S 10
#define INIT_ALLOC_SIZE_M 100
#define INIT_ALLOC_SIZE_L 1000

// Private declarations

typedef struct carrier_t Carrier;
static Carrier *new_carrier(Network *network);
static void realloc_carrier_route_size(Carrier *carrier);
static void realloc_carrier_trip_size(Carrier *carrier);
static void new_route_from_carrier(Carrier *carrier);
static void delete_carrier(Carrier *carrier);

static void handle_stop_facility(xmlNode *xml_node, Carrier *carrier);
static void handle_stop(xmlNode *xml_node, Carrier *carrier);
static void handle_departure(xmlNode *xml_node, Carrier *carrier);
static void handle_route(xmlNode *xml_node, Carrier *carrier);
static void matsim_schedule_parser(xmlNode *xml_node, Carrier *carrier);

static void handle_vehicle_type(xmlNode *xml_node, Network *network);
static void handle_vehicle(xmlNode *xml_node, Network *network);
static void matsim_vehicle_parser(xmlNode *xml_node, Network *network);

// Public implementations

int import_matsim(Network *network, const char *schedule_file,
                  const char *vehicle_file) {
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    // Parse transit vehicles file
    doc = xmlReadFile(vehicle_file, NULL, 0);
    if (doc == NULL) {
        printf("Could not parse file %s.", vehicle_file);
        return 0;
    }
    root_element = xmlDocGetRootElement(doc);
    matsim_vehicle_parser(root_element, network);
    xmlFreeDoc(doc);
    xmlCleanupParser();

    // Parse transit schedule file
    doc = xmlReadFile(schedule_file, NULL, 0);
    if (doc == NULL) {
        printf("Could not parse file %s.", schedule_file);
        return 0;
    }
    root_element = xmlDocGetRootElement(doc);
    Carrier *carrier = new_carrier(network);
    matsim_schedule_parser(root_element, carrier);
    new_route_from_carrier(carrier);  // Add last route
    delete_carrier(carrier);
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return 1;
}

// Private implementations

typedef struct carrier_t {
    Network *network;
    char *route_id;
    Node **nodes;
    int *arrival_offsets;
    int *departure_offsets;
    size_t route_size;
    size_t route_alloc_size;
    const char **trip_ids;
    int *departures;
    struct vehicle_t **vehicles;
    size_t trip_size;
    size_t trip_alloc_size;
    int route_counter;  // Needed for omitting first route element occurrence
} Carrier;

static Carrier *new_carrier(Network *network) {
    Carrier *carrier = (Carrier *)malloc(sizeof(Carrier));
    carrier->network = network;
    carrier->nodes = (Node **)malloc(sizeof(Node *) * INIT_ALLOC_SIZE_S);
    carrier->arrival_offsets = (int *)malloc(sizeof(int) * INIT_ALLOC_SIZE_S);
    carrier->departure_offsets = (int *)malloc(sizeof(int) * INIT_ALLOC_SIZE_S);
    carrier->route_size = 0;
    carrier->route_alloc_size = INIT_ALLOC_SIZE_S;
    carrier->trip_ids =
        (const char **)malloc(sizeof(char *) * INIT_ALLOC_SIZE_M);
    carrier->departures = (int *)malloc(sizeof(int) * INIT_ALLOC_SIZE_M);
    carrier->vehicles = (Vehicle **)malloc(sizeof(Vehicle) * INIT_ALLOC_SIZE_M);
    carrier->trip_size = 0;
    carrier->trip_alloc_size = INIT_ALLOC_SIZE_M;

    // Set counter to zero
    carrier->route_counter = 0;

    return carrier;
}

static void realloc_carrier_route_size(Carrier *carrier) {
    if (carrier->route_size == carrier->route_alloc_size) {
        carrier->route_alloc_size += INIT_ALLOC_SIZE_S;
        // printf("Reallocating route size of reading carrier (%ldx%ld
        // bytes).\n",
        //        carrier->route_alloc_size, sizeof(void *));
        carrier->nodes = (Node **)realloc(
            carrier->nodes, sizeof(Node *) * carrier->route_alloc_size);
        carrier->arrival_offsets = (int *)realloc(
            carrier->arrival_offsets, sizeof(int) * carrier->route_alloc_size);
        carrier->departure_offsets =
            (int *)realloc(carrier->departure_offsets,
                           sizeof(int) * carrier->route_alloc_size);
    }
}

static void realloc_carrier_trip_size(Carrier *carrier) {
    if (carrier->trip_size == carrier->trip_alloc_size) {
        carrier->trip_alloc_size += INIT_ALLOC_SIZE_M;
        // printf("Reallocating trip size of reading carrier (%ldx%ld
        // bytes).\n",
        //        carrier->trip_alloc_size, sizeof(void *));
        carrier->trip_ids = (const char **)realloc(
            carrier->trip_ids, sizeof(char *) * carrier->trip_alloc_size);
        carrier->departures = (int *)realloc(
            carrier->departures, sizeof(int) * carrier->trip_alloc_size);
        carrier->vehicles = (Vehicle **)realloc(
            carrier->vehicles, sizeof(Vehicle *) * carrier->trip_alloc_size);
    }
}

static void new_route_from_carrier(Carrier *carrier) {
    new_route(carrier->network, carrier->route_id, carrier->nodes,
              carrier->arrival_offsets, carrier->departure_offsets,
              carrier->route_size, carrier->trip_ids, carrier->departures,
              carrier->vehicles, carrier->trip_size);
    // Free xmls chars
    xmlFree(carrier->route_id);
    for (int i = 0; i < carrier->trip_size; i++)
        xmlFree((char *)carrier->trip_ids[i]);
    // Reset size for next route
    carrier->route_size = 0;
    carrier->trip_size = 0;
}

static void delete_carrier(Carrier *carrier) {
    free(carrier->nodes);
    free(carrier->arrival_offsets);
    free(carrier->departure_offsets);
    free(carrier->trip_ids);
    free(carrier->departures);
    free(carrier->vehicles);
    free(carrier);
}

static void handle_stop_facility(xmlNode *xml_node, Carrier *carrier) {
    double x, y;
    char *id_tmp = xmlGetProp(xml_node, "id");
    char *x_tmp = xmlGetProp(xml_node, "x");
    char *y_tmp = xmlGetProp(xml_node, "y");
    sscanf(x_tmp, "%lf", &x);
    sscanf(y_tmp, "%lf", &y);
    new_node(carrier->network, id_tmp, x, y);
    xmlFree(id_tmp);
    xmlFree(x_tmp);
    xmlFree(y_tmp);
}

static void handle_stop(xmlNode *xml_node, Carrier *carrier) {
    char *node_id_tmp = xmlGetProp(xml_node, "refId");
    Node *node = get_node(carrier->network, node_id_tmp);
    if (node != NULL) {
        char *arr_off_tmp = xmlGetProp(xml_node, "arrivalOffset");
        char *dep_off_tmp = xmlGetProp(xml_node, "departureOffset");
        carrier->nodes[carrier->route_size] = node;
        carrier->departure_offsets[carrier->route_size] =
            parse_time(dep_off_tmp);
        // Arrival offset must not be set, therefore check if.
        if (arr_off_tmp != NULL) {
            carrier->arrival_offsets[carrier->route_size] =
                parse_time(arr_off_tmp);
        } else {  // Set to departure
            carrier->arrival_offsets[carrier->route_size] =
                carrier->departure_offsets[carrier->route_size];
        }
        ++(carrier->route_size);
        xmlFree(arr_off_tmp);
        xmlFree(dep_off_tmp);
        realloc_carrier_route_size(carrier);
    }
    xmlFree(node_id_tmp);
}

static void handle_departure(xmlNode *xml_node, Carrier *carrier) {
    char *dep_tmp = xmlGetProp(xml_node, "departureTime");
    char *vehicle_id_tmp = xmlGetProp(xml_node, "vehicleRefId");
    Vehicle *vehicle = get_vehicle(carrier->network, vehicle_id_tmp);

    // Add trip attributes
    carrier->trip_ids[carrier->trip_size] = xmlGetProp(xml_node, "id");
    carrier->departures[carrier->trip_size] = parse_time(dep_tmp);
    carrier->vehicles[carrier->trip_size] = vehicle;  // vehicleRefId

    ++(carrier->trip_size);
    xmlFree(dep_tmp);
    xmlFree(vehicle_id_tmp);
    realloc_carrier_trip_size(carrier);
}

static void handle_route(xmlNode *xml_node, Carrier *carrier) {
    // Add new route, if not (!) first route node
    if (carrier->route_counter > 0) {
        new_route_from_carrier(carrier);
    }
    carrier->route_id = xmlGetProp(xml_node, "id");
    ++(carrier->route_counter);
}

static void matsim_schedule_parser(xmlNode *xml_node, Carrier *carrier) {
    while (xml_node) {
        if (xml_node->type == XML_ELEMENT_NODE) {
            if (xmlStrcmp(xml_node->name, "stopFacility") == 0) {
                handle_stop_facility(xml_node, carrier);
            } else if (xmlStrcmp(xml_node->name, "stop") == 0) {
                handle_stop(xml_node, carrier);
            } else if (xmlStrcmp(xml_node->name, "departure") == 0) {
                handle_departure(xml_node, carrier);
            } else if (xmlStrcmp(xml_node->name, "transitRoute") == 0) {
                handle_route(xml_node, carrier);
            }
        }
        matsim_schedule_parser(xml_node->children, carrier);
        xml_node = xml_node->next;
    }
}

static void handle_vehicle_type(xmlNode *xml_node, Network *network) {
    char *id_tmp = xmlGetProp(xml_node, "id");
    int seat_count = 0;

    // Enter vehicleType
    xml_node = xml_node->children;
    // Iterate until capacity
    while (xml_node) {
        if (xml_node->type == XML_ELEMENT_NODE) {
            if (xmlStrcmp(xml_node->name, "capacity") == 0) {
                // Enter capacity
                xml_node = xml_node->children;
            } else if (xmlStrcmp(xml_node->name, "seat_count") == 0) {
                // Get seat_count of capacity
                char *seat_count_tmp = xmlGetProp(xml_node, "persons");
                sscanf(seat_count_tmp, "%d", &seat_count);
                xmlFree(seat_count_tmp);
                break;
            }
        }
        xml_node = xml_node->next;
    }

    new_composition(network, id_tmp, seat_count);
    xmlFree(id_tmp);
}

static void handle_vehicle(xmlNode *xml_node, Network *network) {
    char *id_tmp = xmlGetProp(xml_node, "id");
    char *id_comp_tmp = xmlGetProp(xml_node, "type");
    Composition *comp = get_composition(network, id_comp_tmp);
    if (comp != NULL) {
        Vehicle *v = new_vehicle(network, id_tmp, comp);
    }
    xmlFree(id_tmp);
    xmlFree(id_comp_tmp);
}

static void matsim_vehicle_parser(xmlNode *xml_node, Network *network) {
    while (xml_node) {
        if (xml_node->type == XML_ELEMENT_NODE) {
            if (xmlStrcmp(xml_node->name, "vehicleType") == 0) {
                handle_vehicle_type(xml_node, network);
            } else if (xmlStrcmp(xml_node->name, "vehicle") == 0) {
                handle_vehicle(xml_node, network);
            }
        }
        matsim_vehicle_parser(xml_node->children, network);
        xml_node = xml_node->next;
    }
}
