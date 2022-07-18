/**
 * @brief Input and output of networks and its strutures
 * @file io.c
 * @date: 2022-07-12
 * @author: Merlin Unterfinger
 */

#include <libxml/parser.h>
#include <osurs/io.h>
#include <string.h>

#define MAX_ID_LENGTH 128

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
    char **trip_ids;
    int *departures;
    int *capacities;
    size_t trip_size;
    int route_counter;
} Carrier;

Carrier *new_carrier(Network *network) {
    Carrier *carrier = (Carrier *)malloc(sizeof(Carrier));
    carrier->network = network;
    carrier->route_id = (char *)malloc(sizeof(char) * MAX_ID_LENGTH);
    carrier->nodes = (Node **)malloc(sizeof(Node *) * INIT_ALLOC_SIZE);
    carrier->times = (int *)malloc(sizeof(int) * INIT_ALLOC_SIZE);
    carrier->route_size = 0;

    carrier->trip_ids = (char **)malloc(INIT_ALLOC_SIZE * sizeof(char *));
    for (int i = 0; i < INIT_ALLOC_SIZE; i++) {
        carrier->trip_ids[i] =
            (char *)malloc(sizeof(char) * (MAX_ID_LENGTH + 1));
    }
    carrier->departures = (int *)malloc(sizeof(int) * INIT_ALLOC_SIZE);
    carrier->capacities = (int *)malloc(sizeof(int) * INIT_ALLOC_SIZE);
    carrier->trip_size = 0;

    // Set counter to zero
    carrier->route_counter = 0;

    return carrier;
}

void new_route_from_carrier(Carrier *carrier) {
    new_route(carrier->network, carrier->route_id, carrier->nodes,
              carrier->times, carrier->route_size, carrier->trip_ids,
              carrier->departures, carrier->capacities, carrier->trip_size);
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
    char *id = xmlGetProp(xml_node, "id");
    sscanf(xmlGetProp(xml_node, "x"), "%lf", &x);
    sscanf(xmlGetProp(xml_node, "y"), "%lf", &y);
    new_node(carrier->network, id, x, y);
}

void handle_stop(xmlNode *xml_node, Carrier *carrier) {
    carrier->nodes[carrier->route_size] =
        get_node(carrier->network, xmlGetProp(xml_node, "refId"));
    carrier->times[carrier->route_size] =
        parse_time(xmlGetProp(xml_node, "departureOffset"));
    ++(carrier->route_size);
}

void handle_departure(xmlNode *xml_node, Carrier *carrier) {
    strcpy(carrier->trip_ids[carrier->trip_size], xmlGetProp(xml_node, "id"));
    carrier->departures[carrier->trip_size] =
        parse_time(xmlGetProp(xml_node, "departureTime"));
    carrier->capacities[carrier->trip_size] = 0;  // vehicleRefId
    ++(carrier->trip_size);
}

void handle_route(xmlNode *xml_node, Carrier *carrier) {
    char *curr_route_id = xmlGetProp(xml_node, "id");
    // Add new route, if not first route node
    if (carrier->route_counter > 0) {
        new_route_from_carrier(carrier);
    }
    strcpy(carrier->route_id, curr_route_id);
    ++(carrier->route_counter);
}

void matsim_parser(xmlNode *xml_node, Carrier *carrier) {
    while (xml_node) {
        if (xml_node->type == XML_ELEMENT_NODE) {
            // Leafs
            if (is_leaf(xml_node)) {
                // printf("%s\n", xml_node->name);

                if (xmlStrcmp(xml_node->name, "stopFacility") == 0) {
                    handle_stop_facility(xml_node, carrier);
                } else if (xmlStrcmp(xml_node->name, "stop") == 0) {
                    handle_stop(xml_node, carrier);
                } else if (xmlStrcmp(xml_node->name, "departure") == 0) {
                    handle_departure(xml_node, carrier);
                }

                // Not a leaf
            } else {
                // Route
                if (xmlStrcmp(xml_node->name, "transitRoute") == 0) {
                    handle_route(xml_node, carrier);
                }
            }
        }
        matsim_parser(xml_node->children, carrier);
        xml_node = xml_node->next;
    }
}

int import_matsim(Network *network, const char *schedule_file,
                  const char *vehicle_file) {
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    doc = xmlReadFile(schedule_file, NULL, 0);
    if (doc == NULL) {
        printf("Could not parse the XML file");
        return 1;
    }
    root_element = xmlDocGetRootElement(doc);

    /// HERE WE GO
    Carrier *carrier = new_carrier(network);
    matsim_parser(root_element, carrier);
    // Add last route
    printf("I made it outside of parser!\n");
    new_route_from_carrier(carrier);
    delete_carrier(carrier);
    /// HERE WE END

    // Cleanup xml
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return 0;
}