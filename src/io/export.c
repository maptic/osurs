/**
 * @brief Export network to a file.
 * @file export.c
 * @date: 2022-07-25
 * @author: Merlin Unterfinger
 */

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "osurs/io.h"
#include "utils.h"

#define ENCODING "UTF-8"

int export_network(Network *network, const char *filename) {
    char buf[32];
    int rc;
    xmlTextWriterPtr writer;

    /* Initialize the library and check potential ABI mismatches
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

    // Set indent to 4 spaces.
    rc = xmlTextWriterSetIndent(writer, 1);
    if (rc < 0) {
        printf("testXmlwriterFilename: Error at xmlTextWriterSetIndent\n");
        return 0;
    }
    xmlTextWriterSetIndentString(writer, "    ");

    /* Start an element named "network". Since this is the first
     * element, this will be the root element of the document. */
    xmlTextWriterStartElement(writer, "network");
    sprintf(buf, "%ld", network->node_counter);
    xmlTextWriterWriteAttribute(writer, "nodes", buf);
    sprintf(buf, "%ld", network->composition_counter);
    xmlTextWriterWriteAttribute(writer, "compositions", buf);
    sprintf(buf, "%ld", network->vehicle_counter);
    xmlTextWriterWriteAttribute(writer, "vehicles", buf);
    sprintf(buf, "%ld", network->route_counter);
    xmlTextWriterWriteAttribute(writer, "routes", buf);

    // Nodes
    xmlTextWriterStartElement(writer, "nodes");
    for (size_t i = 0; i < network->node_counter; ++i) {
        Node *node = network->nodes[i];
        xmlTextWriterStartElement(writer, "node");
        sprintf(buf, "%s", node->id);
        xmlTextWriterWriteAttribute(writer, "id", buf);
        sprintf(buf, "%.5f", node->x);
        xmlTextWriterWriteAttribute(writer, "x", buf);
        sprintf(buf, "%.5f", node->y);
        xmlTextWriterWriteAttribute(writer, "y", buf);
        sprintf(buf, "%ld", node->route_counter);
        xmlTextWriterWriteAttribute(writer, "routes", buf);
        xmlTextWriterEndElement(writer);
    }
    xmlTextWriterEndElement(writer);

    // Compositions
    xmlTextWriterStartElement(writer, "compositions");
    for (size_t i = 0; i < network->composition_counter; ++i) {
        Composition *comp = network->compositions[i];
        xmlTextWriterStartElement(writer, "composition");
        sprintf(buf, "%s", comp->id);
        xmlTextWriterWriteAttribute(writer, "id", buf);
        sprintf(buf, "%d", comp->seat_count);
        xmlTextWriterWriteAttribute(writer, "seat_count", buf);
        xmlTextWriterEndElement(writer);
    }
    xmlTextWriterEndElement(writer);

    // Vehicles
    xmlTextWriterStartElement(writer, "vehicles");
    for (size_t i = 0; i < network->vehicle_counter; ++i) {
        Vehicle *vehicle = network->vehicles[i];
        xmlTextWriterStartElement(writer, "vehicle");
        sprintf(buf, "%s", vehicle->id);
        xmlTextWriterWriteAttribute(writer, "id", buf);
        sprintf(buf, "%s", vehicle->composition->id);
        xmlTextWriterWriteAttribute(writer, "cid", buf);
        xmlTextWriterEndElement(writer);
    }
    xmlTextWriterEndElement(writer);

    // Routes
    xmlTextWriterStartElement(writer, "routes");
    for (size_t i = 0; i < network->route_counter; ++i) {
        Route *route = network->routes[i];
        Stop *curr_stop = route->root_stop;
        Trip *curr_trip = route->root_trip;

        // Route
        xmlTextWriterStartElement(writer, "route");
        sprintf(buf, "%s", route->id);
        xmlTextWriterWriteAttribute(writer, "id", buf);
        sprintf(buf, "%ld", route->route_size);
        xmlTextWriterWriteAttribute(writer, "stops", buf);
        sprintf(buf, "%ld", route->trip_size);
        xmlTextWriterWriteAttribute(writer, "trips", buf);

        // Stops
        xmlTextWriterStartElement(writer, "stops");
        while (curr_stop != NULL) {
            xmlTextWriterStartElement(writer, "stop");
            compose_time(buf, curr_stop->arrival_offset);
            xmlTextWriterWriteAttribute(writer, "arr_off", buf);
            compose_time(buf, curr_stop->departure_offset);
            xmlTextWriterWriteAttribute(writer, "dep_off", buf);
            sprintf(buf, "%s", curr_stop->node->id);
            xmlTextWriterWriteAttribute(writer, "nid", buf);
            xmlTextWriterEndElement(writer);
            curr_stop = curr_stop->next;
        }
        xmlTextWriterEndElement(writer);

        // Trips
        xmlTextWriterStartElement(writer, "trips");
        while (curr_trip != NULL) {
            xmlTextWriterStartElement(writer, "trip");
            sprintf(buf, "%s", curr_trip->id);
            xmlTextWriterWriteAttribute(writer, "id", buf);
            compose_time(buf, curr_trip->departure);
            xmlTextWriterWriteAttribute(writer, "dep", buf);
            compose_time(buf, curr_trip->arrival);
            xmlTextWriterWriteAttribute(writer, "arr", buf);
            sprintf(buf, "%s", curr_trip->vehicle->id);
            xmlTextWriterWriteAttribute(writer, "vid", buf);
            xmlTextWriterEndElement(writer);
            curr_trip = curr_trip->next;
        }
        xmlTextWriterEndElement(writer);

        // Close route
        xmlTextWriterEndElement(writer);
    }
    xmlTextWriterEndElement(writer);

    /* Close the root element "network" named. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf(
            "testXmlwriterFilename: Error at "
            "xmlTextWriterEndElement\n");
        return 0;
    }

    // End the document.
    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0) {
        printf(
            "testXmlwriterFilename: Error at "
            "xmlTextWriterEndDocument\n");
        return 0;
    }

    // Cleanup.
    xmlFreeTextWriter(writer);

    return 1;
}

int export_reservations(Network *network, const char *filename) {
    char buf[32];
    int rc;
    xmlTextWriterPtr writer;

    /* Initialize the library and check potential ABI mismatches
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

    // Set indent to 4 spaces.
    rc = xmlTextWriterSetIndent(writer, 1);
    if (rc < 0) {
        printf("testXmlwriterFilename: Error at xmlTextWriterSetIndent\n");
        return 0;
    }
    xmlTextWriterSetIndentString(writer, "    ");

    /* Start an element named "reservations". Since this is the first
     * element, this will be the root element of the document. */
    xmlTextWriterStartElement(writer, "reservations");

    // Routes
    for (size_t i = 0; i < network->route_counter; ++i) {
        Route *route = network->routes[i];
        Stop *curr_stop = route->root_stop;
        Trip *curr_trip = route->root_trip;

        // Trips
        while (curr_trip != NULL) {
            // Reservations
            if (curr_trip->reservation_counter > 0) {
                for (size_t i = 0; i < curr_trip->reservation_counter; ++i) {
                    Reservation *res = curr_trip->reservations[i];
                    xmlTextWriterStartElement(writer, "reservation");
                    sprintf(buf, "%s", route->id);
                    xmlTextWriterWriteAttribute(writer, "rid", buf);
                    sprintf(buf, "%s", curr_trip->id);
                    xmlTextWriterWriteAttribute(writer, "tid", buf);
                    sprintf(buf, "%s", res->orig->node->id);
                    xmlTextWriterWriteAttribute(writer, "nid_orig", buf);
                    sprintf(buf, "%s", res->dest->node->id);
                    xmlTextWriterWriteAttribute(writer, "nid_dest", buf);
                    sprintf(buf, "%d", res->seats);
                    xmlTextWriterWriteAttribute(writer, "seats", buf);
                    xmlTextWriterEndElement(writer);
                }
            }
            curr_trip = curr_trip->next;
        }
    }

    /* Close the root element "reservations". */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf(
            "testXmlwriterFilename: Error at "
            "xmlTextWriterEndElement\n");
        return 0;
    }

    // End the document.
    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0) {
        printf(
            "testXmlwriterFilename: Error at "
            "xmlTextWriterEndDocument\n");
        return 0;
    }

    // Cleanup.
    xmlFreeTextWriter(writer);

    return 1;
}
