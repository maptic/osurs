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
    printf("  - Trip <departure=%d, capacity=%d>\n", trip->departure,
           trip->capacity);
}

void print_route(Route *route) {
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

void print_xml(xmlNode *node, int indent_len) {
    while (node) {
        if (node->type == XML_ELEMENT_NODE) {
            printf("%*c%s:%s\n", indent_len * 2, '-', node->name,
                   is_leaf(node) ? xmlNodeGetContent(node)
                                 : xmlGetProp(node, "id"));
        }
        print_xml(node->children, indent_len + 1);
        node = node->next;
    }
}

int import_matsim2(Network *network, const char *schedule_file,
                   const char *vehicle_file) {
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    doc = xmlReadFile(schedule_file, NULL, 0);

    if (doc == NULL) {
        printf("Could not parse the XML file");
        return 1;
    }

    root_element = xmlDocGetRootElement(doc);

    print_xml(root_element, 1);

    xmlFreeDoc(doc);

    xmlCleanupParser();

    return 0;
}

void parse_xml(xmlNode *xml_node, Network *network) {
    // Declare route attributes
    const char *route_id;
    // const char *route_mode; // transportMode
    Node *nodes[INIT_ALLOC_SIZE];
    int times[INIT_ALLOC_SIZE];
    size_t route_size = 0;

    // Declare trip attributes
    const char *trip_ids[INIT_ALLOC_SIZE];
    int departures[INIT_ALLOC_SIZE];
    int capacities[INIT_ALLOC_SIZE];
    size_t trip_size = 0;

    while (xml_node) {
        if (xml_node->type == XML_ELEMENT_NODE) {
            // Leafs
            if (is_leaf(xml_node)) {
                printf("O - Leaf: %s - %s\n", xml_node->name,
                       xmlGetProp(xml_node, "id"));
                // xmlNodeGetContent(node));

                // Parse network node
                if (xmlStrcmp(xml_node->name, "stopFacility") == 0) {
                    double x, y;
                    char *id = xmlGetProp(xml_node, "id");
                    sscanf(xmlGetProp(xml_node, "x"), "%lf", &x);
                    sscanf(xmlGetProp(xml_node, "y"), "%lf", &y);
                    new_node(network, id, x, y);
                } else if (xmlStrcmp(xml_node->name, "stop") == 0) {
                    nodes[route_size] = get_node(network, xmlGetProp(xml_node, "refId"));
                    times[route_size] = 0;
                    ++route_size;
                } else if (xmlStrcmp(xml_node->name, "departure ") == 0) {
                    trip_ids[trip_size] = xmlGetProp(xml_node, "id");
                    departures[trip_size] = 0;  // departureTime
                    capacities[trip_size] = 0;  // vehicleRefId
                    ++trip_size;
                }

                // No leafs
            } else {
                // Route
                if (xmlStrcmp(xml_node->name, "transitRoute") == 0) {
                    const char *curr_route_id = xmlGetProp(xml_node, "id");
                    if (xmlStrcmp(curr_route_id, route_id) != 0) {
                        if (route_id != NULL) {
                            // Add route to network!!!
                            printf("Adding route to network!!!\n");
                            // Reset counters
                            route_size = 0;
                            trip_size = 0;
                        }
                        route_id = curr_route_id;
                    }
                    printf("--> Got route: %s\n", route_id);
                }

                // printf("X - No leaf: %s - %s\n", xml_node->name,
                //       xmlGetProp(xml_node, "id"));
            }
        }
        parse_xml(xml_node->children, network);
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
    parse_xml(root_element, network);
    /// HERE WE END

    xmlFreeDoc(doc);

    xmlCleanupParser();

    return 0;
}