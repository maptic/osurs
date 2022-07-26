/**
 * @brief Import a network file.
 * @file import.c
 * @date: 2022-07-19
 * @author: Merlin Unterfinger
 */

#include <libxml/parser.h>
#include <osurs/io.h>

// Private declarations

static void network_parser(xmlNode *xml_node, Network *network);

// Public definitions

int import_network(Network *network, const char *filename) {
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    // Parse transit vehicules file
    doc = xmlReadFile(filename, NULL, 0);
    if (doc == NULL) {
        printf("Could not parse file %s.", filename);
        return 0;
    }
    root_element = xmlDocGetRootElement(doc);
    // network_parser(root_element, network);
    xmlFreeDoc(doc);
    xmlCleanupParser();
}

// Private definitions

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

// static void network_parser(xmlNode *xml_node, Network *network) {
//     while (xml_node) {
//         if (xml_node->type == XML_ELEMENT_NODE) {
//             if (xmlStrcmp(xml_node->name, "vehicleType") == 0) {
//                 handle_vehicle_type(xml_node, network);
//             } else if (xmlStrcmp(xml_node->name, "vehicle") == 0) {
//                 handle_vehicle(xml_node, network);
//             }
//         }
//         matsim_vehicle_parser(xml_node->children, network);
//         xml_node = xml_node->next;
//     }
// }

int is_leaf(xmlNode *node) {
    xmlNode *child = node->children;
    while (child) {
        if (child->type == XML_ELEMENT_NODE) return 0;
        child = child->next;
    }

    return 1;
}
