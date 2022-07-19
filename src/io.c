/**
 * @brief Input and output of networks and its strutures
 * @file io.c
 * @date: 2022-07-12
 * @author: Merlin Unterfinger
 */

#include <libxml/parser.h>
#include <osurs/io.h>
#include <string.h>

// Print methods

// Needs buffer size of char[9], due to string terminating sign.
int compose_time(char *buffer, int time) {
    int h, m, s;
    h = (time / 3600);
    m = (time - (3600 * h)) / 60;
    s = (time - (3600 * h) - (m * 60));
    sprintf(buffer, "%.2d:%.2d:%.2d", h, m, s);
    return 0;
}

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

// File I/O

int print_cwd() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    } else {
        perror("getcwd() error");
        return 0;
    }
    return 1;
}

int print_file(const char *filename) {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("fopen() error");
        return 0;
    }
    while ((read = getline(&line, &len, fp)) != -1) {
        // Optionally print length: printf("Retrieved line of length %zu:\n",
        // read);
        printf("%s", line);
    }

    fclose(fp);
    if (line) free(line);
    return 1;
}

// XML

int is_leaf(xmlNode *node) {
    xmlNode *child = node->children;
    while (child) {
        if (child->type == XML_ELEMENT_NODE) return 0;
        child = child->next;
    }

    return 1;
}

int parse_time(char *time) {
    int h, m, s;
    sscanf(time, "%d:%d:%d", &h, &m, &s);
    return h * HOURS + m * MINUTES + s;
}

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
    int route_counter;  // Needed for ommiting first route element occurence
} Carrier;

Carrier *new_carrier(Network *network) {
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

void realloc_carrier_route_size(Carrier *carrier) {
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

void realloc_carrier_trip_size(Carrier *carrier) {
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

void new_route_from_carrier(Carrier *carrier) {
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

void delete_carrier(Carrier *carrier) {
    free(carrier->nodes);
    free(carrier->arrival_offsets);
    free(carrier->departure_offsets);
    free(carrier->trip_ids);
    free(carrier->departures);
    free(carrier->vehicles);
    free(carrier);
}

void handle_stop_facility(xmlNode *xml_node, Carrier *carrier) {
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

void handle_stop(xmlNode *xml_node, Carrier *carrier) {
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

void handle_departure(xmlNode *xml_node, Carrier *carrier) {
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

void handle_route(xmlNode *xml_node, Carrier *carrier) {
    // Add new route, if not (!) first route node
    if (carrier->route_counter > 0) {
        new_route_from_carrier(carrier);
    }
    carrier->route_id = xmlGetProp(xml_node, "id");
    ++(carrier->route_counter);
}

void matsim_schedule_parser(xmlNode *xml_node, Carrier *carrier) {
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

void handle_vehicle_type(xmlNode *xml_node, Network *network) {
    char *id_tmp = xmlGetProp(xml_node, "id");
    int seats = 0;

    // Enter vehicleType
    xml_node = xml_node->children;
    // Iterate until capacity
    while (xml_node) {
        if (xml_node->type == XML_ELEMENT_NODE) {
            if (xmlStrcmp(xml_node->name, "capacity") == 0) {
                // Enter capacity
                xml_node = xml_node->children;
            } else if (xmlStrcmp(xml_node->name, "seats") == 0) {
                // Get seats of capacity
                char *seats_tmp = xmlGetProp(xml_node, "persons");
                sscanf(seats_tmp, "%d", &seats);
                xmlFree(seats_tmp);
                break;
            }
        }
        xml_node = xml_node->next;
    }

    new_composition(network, id_tmp, seats);
    xmlFree(id_tmp);
}

void handle_vehicle(xmlNode *xml_node, Network *network) {
    char *id_tmp = xmlGetProp(xml_node, "id");
    char *id_comp_tmp = xmlGetProp(xml_node, "type");
    Composition *comp = get_composition(network, id_comp_tmp);
    if (comp != NULL) {
        Vehicle *v = new_vehicle(network, id_tmp, comp);
    }
    xmlFree(id_tmp);
    xmlFree(id_comp_tmp);
}

void matsim_vehicle_parser(xmlNode *xml_node, Network *network) {
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

int import_matsim(Network *network, const char *schedule_file,
                  const char *vehicle_file) {
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    // Parse transit vehicules file
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