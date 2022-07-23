/**
 * @brief Export network to a file.
 * @file export.c
 * @date: 2022-07-22
 * @author: Merlin Unterfinger
 */

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <osurs/io.h>

#define ENCODING "UTF-8"

int export_network(Network *network, const char *filename) {
    int rc;
    xmlTextWriterPtr writer;

    /* this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    /* Create a new XmlWriter for uri, with no compression. */
    writer = xmlNewTextWriterFilename(filename, 0);
    if (writer == NULL) {
        printf("testXmlwriterFilename: Error creating the xml writer\n");
        return 0;
    }

    /* Start the document with the xml default for the version,
     * encoding UTF-8 and the default for the standalone
     * declaration. */
    rc = xmlTextWriterStartDocument(writer, NULL, ENCODING, NULL);
    if (rc < 0) {
        printf("testXmlwriterFilename: Error at xmlTextWriterStartDocument\n");
        return 0;
    }

    /* Start an element named "network". Since this is the first
     * element, this will be the root element of the document. */
    rc = xmlTextWriterStartElement(writer, "network");
    xmlTextWriterWriteElement(writer, "nodes", network->node_counter);
    xmlTextWriterWriteElement(writer, "composition", network->composition_counter);
    xmlTextWriterWriteElement(writer, "vehicles", network->vehicle_counter);
    xmlTextWriterWriteElement(writer, "routes", network->route_counter);

    // Network content.

    // Nodes
    xmlTextWriterStartElement(writer, "nodes");
    xmlTextWriterWriteElement(writer, "node", "ha");
    xmlTextWriterEndElement(writer);

    // Compositions
    xmlTextWriterStartElement(writer, "compositions");
    xmlTextWriterWriteElement(writer, "composition", "ha");
    xmlTextWriterEndElement(writer);

    // Vehicles
    xmlTextWriterStartElement(writer, "vehicles");
    xmlTextWriterWriteElement(writer, "vehicle", "ha");
    xmlTextWriterEndElement(writer);

    // Routes
    xmlTextWriterStartElement(writer, "routes");
    xmlTextWriterWriteElement(writer, "route", "ha");
    xmlTextWriterEndElement(writer);

    /* Close the root element "network" named. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf("testXmlwriterFilename: Error at xmlTextWriterEndElement\n");
        return 1;
    }

    // End the document.
    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0) {
        printf("testXmlwriterFilename: Error at xmlTextWriterEndDocument\n");
        return 0;
    }

    // Cleanup.
    xmlFreeTextWriter(writer);

    return 1;
}
