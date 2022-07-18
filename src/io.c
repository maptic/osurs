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

void print_node(Node *node) {
    printf("- Node <id=%s, x=%.3f, y=%.3f, routes=%ld>\n", node->id, node->x,
           node->y, node->route_counter);
}

void print_stop(Stop *stop) {
    printf("  - Stop <node_id=%s, last=%s, next=%s>]\n", stop->node->id,
           stop->last != NULL ? stop->last->node->id : "NA",
           stop->next != NULL ? stop->next->node->id : "NA");
}

void print_trip(Trip *trip) {
    printf("  - Trip <id=%s, departure=%d, capacity=%d>\n", trip->id,
           trip->departure, trip->capacity);
}

void print_route(Route *route) {
    printf("- Route '%s'\n", route->id);
    printf("- Route <Stops>:\n");
    Stop *curr_stop = route->root_stop;
    while (curr_stop != NULL) {
        print_stop(curr_stop);
        curr_stop = curr_stop->next;
    }
    printf("- Route <Trips>:\n");
    Trip *curr_trip = route->root_trip;
    while (curr_trip != NULL) {
        print_trip(curr_trip);
        curr_trip = curr_trip->next;
    }
}

void print_network(Network *network) {
    printf("Network <Nodes>:\n");
    for (size_t i = 0; i < network->node_counter; ++i) {
        print_node(network->nodes[i]);
    }
    printf("Network <Routes>:\n");
    for (size_t i = 0; i < network->route_counter; ++i) {
        print_route(network->routes[i]);
    }
    printf("Compositions: %ld, vehicles: %ld\n", network->composition_counter,
           network->vehicle_counter);
}

void print_connection(Connection *connection) {
    Connection *curr_connection = connection;
    if (connection == NULL) {
        printf("No connection found.\n");
        return;
    }
    printf("Connections for %s --> %s:\n", connection->orig->node->id,
           connection->dest->node->id);
    while (curr_connection != NULL) {
        printf("- Connection<departure=%d, arrival=%d, available=%d>\n",
               curr_connection->departure, curr_connection->arrival,
               curr_connection->available);
        curr_connection = curr_connection->next;
    }
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
        return 1;
    }
    return 0;
}

int print_file(const char *filename) {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("fopen() error");
        return 1;
    }
    while ((read = getline(&line, &len, fp)) != -1) {
        // Optionally print length: printf("Retrieved line of length %zu:\n",
        // read);
        printf("%s", line);
    }

    fclose(fp);
    if (line) free(line);
    return 0;
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
    int *times;
    size_t route_size;
    size_t route_alloc_size;
    const char **trip_ids;
    int *departures;
    int *capacities;
    size_t trip_size;
    size_t trip_alloc_size;
    int route_counter;  // Needed for ommiting first route element occurence
} Carrier;

Carrier *new_carrier(Network *network) {
    Carrier *carrier = (Carrier *)malloc(sizeof(Carrier));
    carrier->network = network;
    carrier->nodes = (Node **)malloc(sizeof(Node *) * INIT_ALLOC_SIZE);
    carrier->times = (int *)malloc(sizeof(int) * INIT_ALLOC_SIZE);
    carrier->route_size = 0;
    carrier->route_alloc_size = INIT_ALLOC_SIZE;
    carrier->trip_ids = (const char **)malloc(sizeof(char *) * INIT_ALLOC_SIZE);
    carrier->departures = (int *)malloc(sizeof(int) * INIT_ALLOC_SIZE);
    carrier->capacities = (int *)malloc(sizeof(int) * INIT_ALLOC_SIZE);
    carrier->trip_size = 0;
    carrier->trip_alloc_size = INIT_ALLOC_SIZE;

    // Set counter to zero
    carrier->route_counter = 0;

    return carrier;
}

void realloc_carrier_route_size(Carrier *carrier) {
    if (carrier->route_size == carrier->route_alloc_size) {
        printf("Reallocating route size.\n");
        carrier->route_alloc_size += INIT_ALLOC_SIZE;
        carrier->nodes = (Node **)realloc(
            carrier->nodes, sizeof(Node *) * carrier->route_alloc_size);
        carrier->times = (int *)realloc(
            carrier->times, sizeof(int) * carrier->route_alloc_size);
    }
}

void realloc_carrier_trip_size(Carrier *carrier) {
    if (carrier->trip_size == carrier->trip_alloc_size) {
        printf("Reallocating trip size.\n");
        carrier->trip_alloc_size += INIT_ALLOC_SIZE;
        carrier->trip_ids = (const char **)realloc(
            carrier->trip_ids, sizeof(char *) * carrier->trip_alloc_size);
        carrier->departures = (int *)realloc(
            carrier->departures, sizeof(int) * carrier->trip_alloc_size);
        carrier->capacities = (int *)realloc(
            carrier->capacities, sizeof(int) * carrier->trip_alloc_size);
    }
}

void new_route_from_carrier(Carrier *carrier) {
    new_route(carrier->network, carrier->route_id, carrier->nodes,
              carrier->times, carrier->route_size, carrier->trip_ids,
              carrier->departures, carrier->capacities, carrier->trip_size);
    // Free xmls chars
    free(carrier->route_id);
    for (int i = 0; i < carrier->trip_size; i++)
        free((char *)carrier->trip_ids[i]);
    // Reset size for next route
    carrier->route_size = 0;
    carrier->trip_size = 0;
}

void delete_carrier(Carrier *carrier) {
    free(carrier->nodes);
    free(carrier->times);
    free(carrier->trip_ids);
    free(carrier->departures);
    free(carrier->capacities);
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
    free(id_tmp);
    free(x_tmp);
    free(y_tmp);
}

void handle_stop(xmlNode *xml_node, Carrier *carrier) {
    char *node_id_tmp = xmlGetProp(xml_node, "refId");
    Node *node = get_node(carrier->network, node_id_tmp);
    if (node != NULL) {
        char *dep_off_tmp = xmlGetProp(xml_node, "departureOffset");
        carrier->nodes[carrier->route_size] = node;
        carrier->times[carrier->route_size] = parse_time(dep_off_tmp);
        ++(carrier->route_size);
        free(dep_off_tmp);
        realloc_carrier_route_size(carrier);
    }
    free(node_id_tmp);
}

void handle_departure(xmlNode *xml_node, Carrier *carrier) {
    char *dep_tmp = xmlGetProp(xml_node, "departureTime");
    carrier->trip_ids[carrier->trip_size] = xmlGetProp(xml_node, "id");
    carrier->departures[carrier->trip_size] = parse_time(dep_tmp);
    carrier->capacities[carrier->trip_size] = 0;  // vehicleRefId
    ++(carrier->trip_size);
    free(dep_tmp);
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
                free(seats_tmp);
                break;
            }
        }
        xml_node = xml_node->next;
    }

    new_composition(network, id_tmp, seats);
    free(id_tmp);
}

void handle_vehicle(xmlNode *xml_node, Network *network) {
    char *id_tmp = xmlGetProp(xml_node, "id");
    char *id_comp_tmp = xmlGetProp(xml_node, "type");
    Composition *comp = get_composition(network, id_comp_tmp);
    if (comp != NULL) {
        Vehicle *v = new_vehicle(network, id_tmp, comp);
    }
    free(id_tmp);
    free(id_comp_tmp);
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
        return 1;
    }
    root_element = xmlDocGetRootElement(doc);
    matsim_vehicle_parser(root_element, network);
    xmlFreeDoc(doc);
    xmlCleanupParser();

    // Parse transit schedule file
    doc = xmlReadFile(schedule_file, NULL, 0);
    if (doc == NULL) {
        printf("Could not parse file %s.", schedule_file);
        return 1;
    }
    root_element = xmlDocGetRootElement(doc);
    Carrier *carrier = new_carrier(network);
    matsim_schedule_parser(root_element, carrier);
    new_route_from_carrier(carrier);  // Add last route
    delete_carrier(carrier);
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return 0;
}