/**
 * @brief Import a network file.
 * @file import.c
 * @date: 2022-07-19
 * @author: Merlin Unterfinger
 */

#include <libxml/parser.h>
#include <osurs/io.h>

int import_network(Network *network, const char *filename) {}

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

int is_leaf(xmlNode *node) {
    xmlNode *child = node->children;
    while (child) {
        if (child->type == XML_ELEMENT_NODE) return 0;
        child = child->next;
    }

    return 1;
}
