#include <gtest/gtest.h>

extern "C" {
#include <osurs/network.h>
}

/**
 * @brief Test network creation.
 *
 * Create simplified network of IC lines in Switzerland.
 *
 * Stations:
 * - BS: Basel SBB, 2'611'209.0, 1'266'368.0
 * - BN: Bern, 2'600'082.9, 1'199'823.0
 * - BI: Biel/Bienne, 2'585'140.5, 1'220'206.0
 * - BR: Brig, 2'642'319.8, 1'129'979.0
 * - CH: Chur, 2'759'374.3, 1'191'256.0
 * - DMT: Delémont, 2'593'319.8, 1'245'684.0
 * - GD: Arth-Goldau, 2'684'423.0, 1'211'495.8
 * - GE: Genève-Aéroport, 2'497'658.9, 1'120'991.3
 * - IO: Interlaken Ost, 2'632'921.9, 1'171'162.3
 * - LS: Lausanne, 2'537'873.1, 1'152'046.5
 * - LG: Lugano, 2'716'797.4, 1'095'991.5
 * - LZ: Luzern, 2'666'221.9, 1'211'276.3
 * - RH: Romanshorn, 2'746'046.6, 1'270'119.3
 * - SG: St. Gallen, 2'745'685.3, 1'254'299.5
 * - SH: Schaffhausen, 2'689'629.9, 1'283'767.0
 * - ZG: Zug, 2'681'612.9, 1'225'323.5
 * - ZUE: Zürich HB, 2'683'036.5, 1'248'101.0
 *
 * Lines:
 * - IC 1: GE - LS - BN - ZUE - SG,
 * - IC 2: ZUE - ZG - GD - LG
 * - IC 3: BS - ZUE - CH
 * - IC 4: ZUE - SH
 * - IC 5: GE - LS - BI - ZUE - SG
 * - IC 6: BS - OL - BN - BR
 * - IC 8: BR - BN - ZUE - RH
 * - IC 21: BS - LZ - GD - LG
 * - IC 51: BS - DMT - BI
 * - IC 61: BS - BN - IO
 *
 * Trains:
 * - IC 2000: IC 6, IC 8, IC 61
 * - ICN: IC 5, IC 21, IC 51
 * - FV-Dosto: IC 1
 * - Giruno: IC 2
 *
 */
TEST(NetworkTest, Create) {
    // Initialize network
    Network *network = new_network();

    // Stations
    Node *BS = new_node(network, "Basel SBB", 2611209.0, 1266368.0);
    Node *BN = new_node(network, "Bern", 2600082.9, 1199823.0);
    Node *BI = new_node(network, "Biel/Bienne", 2585140.5, 1220206.0);
    Node *BR = new_node(network, "Brig", 2642319.8, 1129979.0);
    Node *CH = new_node(network, "Chur", 2759374.3, 1191256.0);
    Node *DMT = new_node(network, "Delémont", 2593319.8, 1245684.0);
    Node *GD = new_node(network, "Arth-Goldau", 2684423.0, 1211495.8);
    Node *GE = new_node(network, "Genève-Aéroport", 2497658.9, 1120991.3);
    Node *IO = new_node(network, "Interlaken Ost", 2632921.9, 1171162.3);
    Node *LS = new_node(network, "Lausanne", 2537873.1, 1152046.5);
    Node *LG = new_node(network, "Lugano", 2716797.4, 1095991.5);
    Node *LZ = new_node(network, "Luzern", 2666221.9, 1211276.3);
    Node *OL = new_node(network, "Olten", 2635449.0, 1244666.0);
    Node *RH = new_node(network, "Romanshorn", 2746046.6, 1270119.3);
    Node *SG = new_node(network, "St. Gallen", 2745685.3, 1254299.5);
    Node *SH = new_node(network, "Schaffhausen", 2689629.9, 1283767.0);
    Node *ZG = new_node(network, "Zug", 2681612.9, 1225323.5);
    Node *ZUE = new_node(network, "Zürich HB", 2683036.5, 1248101.0);
    EXPECT_EQ(network->node_counter, 18);

    // Compositions
    Composition *ic2 =
        new_composition(network, "IC2000", 500);  // IC 6, IC 8, IC 61
    Composition *icn =
        new_composition(network, "ICN", 476);  // IC 5, IC 21, IC 51
    Composition *fvd = new_composition(network, "FV-Dosto", 606);  // IC 1
    Composition *gir = new_composition(network, "Giruno", 405);    // IC 2
    EXPECT_EQ(network->composition_counter, 4);

    // Vehicles
    Vehicle *ic2_1 = new_vehicle(network, "ic2-1", ic2);
    Vehicle *ic2_2 = new_vehicle(network, "ic2-2", ic2);
    Vehicle *icn_1 = new_vehicle(network, "icn-1", icn);
    Vehicle *icn_2 = new_vehicle(network, "icn-2", icn);
    Vehicle *fvd_1 = new_vehicle(network, "fvd-1", fvd);
    Vehicle *fvd_2 = new_vehicle(network, "fvd-2", fvd);
    Vehicle *gir_1 = new_vehicle(network, "gir-1", gir);
    Vehicle *gir_2 = new_vehicle(network, "gir-2", gir);
    EXPECT_EQ(network->vehicle_counter, 8);

    // Add routes for line IC 1
    const char *route_id = "ic-1-we";
    Node *nodes[] = {GE, LS, BN, ZUE, SG};
    int arrival_offsets[] = {0, 35 * MINUTES, 1 * HOURS + 44 * MINUTES,
                             2 * HOURS + 41 * MINUTES,
                             3 * HOURS + 42 * MINUTES};
    int departure_offsets[] = {0, 38 * MINUTES, 1 * HOURS + 47 * MINUTES,
                               2 * HOURS + 44 * MINUTES,
                               3 * HOURS + 43 * MINUTES};
    size_t route_size = 5;

    // Define trip attributes
    const char *trip_ids[] = {"ic-1-we-1", "ic-1-we-2", "ic-1-we-3",
                              "ic-1-we-4", "ic-1-we-5"};
    int departures[] = {6 * HOURS, 9 * HOURS, 12 * HOURS, 15 * HOURS,
                        18 * HOURS};
    Vehicle *vehicles[] = {fvd_1, fvd_2, fvd_1, fvd_2, fvd_1};
    size_t trip_size = 5;

    // Create route
    new_route(network, route_id, nodes, arrival_offsets,
              departure_offsets,  // Route properties
              route_size, trip_ids, departures, vehicles,
              trip_size  // Trip properties
    );
    EXPECT_EQ(network->route_counter, 1);
    for (size_t i = 0; i < route_size; ++i) {
        EXPECT_EQ(nodes[i]->route_counter, 1);
    }

    // Reverse direction
    const char *route_id2 = "ic-1-ew";
    Node *nodes2[] = {SG, ZUE, BN, LS, GE};
    int arrival_offsets2[] = {0, 59 * MINUTES, 1 * HOURS + 58 * MINUTES,
                              3 * HOURS + 4 * MINUTES,
                              3 * HOURS + 42 * MINUTES};
    int departure_offsets2[] = {
        0, 1 * HOURS + 2 * MINUTES, 2 * HOURS + 1 * MINUTES,
        3 * HOURS + 7 * MINUTES, 3 * HOURS + 42 * MINUTES};

    // Define trip attributes
    const char *trip_ids2[] = {"ic-1-ew-1", "ic-1-ew-2", "ic-1-ew-3",
                               "ic-1-ew-4", "ic-1-ew-5"};
    Vehicle *vehicles2[] = {fvd_2, fvd_1, fvd_2, fvd_1, fvd_2};

    // Create route
    new_route(network, route_id2, nodes2, arrival_offsets2,
              departure_offsets2,  // Route properties
              route_size, trip_ids2, departures, vehicles2,
              trip_size  // Trip properties
    );
    EXPECT_EQ(network->route_counter, 2);
    for (size_t i = 0; i < route_size; ++i) {
        EXPECT_EQ(nodes[i]->route_counter, 2);
    }

    // Add routes for line IC 2
    const char *route_id3 = "ic-2-ns";
    Node *nodes3[] = {ZUE, ZG, GD, LG};
    int arrival_offsets3[] = {0, 26 * MINUTES, 45 * MINUTES,
                              1 * HOURS + 57 * MINUTES};
    int departure_offsets3[] = {0, 29 * MINUTES, 48 * MINUTES,
                                1 * HOURS + 57 * MINUTES};
    size_t route_size3 = 4;

    // Define trip attributes
    const char *trip_ids3[] = {"ic-2-ns-1", "ic-2-ns-2", "ic-2-ns-3",
                               "ic-2-ns-4", "ic-2-ns-5"};
    int departures3[] = {6 * HOURS + 30 * MINUTES, 9 * HOURS + 30 * MINUTES,
                         12 * HOURS + 30 * MINUTES, 15 * HOURS + 30 * MINUTES,
                         18 * HOURS + 30 * MINUTES};
    Vehicle *vehicles3[] = {gir_1, gir_2, gir_1, gir_2, gir_1};

    // Create route
    new_route(network, route_id3, nodes3, arrival_offsets3,
              departure_offsets3,  // Route properties
              route_size3, trip_ids3, departures3, vehicles3,
              trip_size  // Trip properties
    );
    EXPECT_EQ(network->route_counter, 3);

    // Reverse direction
    const char *route_id4 = "ic-2-sn";
    Node *nodes4[] = {LG, GD, ZG, ZUE};
    int arrival_offsets4[] = {0, 1 * HOURS + 10 * MINUTES,
                              1 * HOURS + 31 * MINUTES,
                              1 * HOURS + 57 * MINUTES};
    int departure_offsets4[] = {0, 1 * HOURS + 13 * MINUTES, 1 * HOURS + 34,
                                1 * HOURS + 57 * MINUTES};

    // Define trip attributes
    const char *trip_ids4[] = {"ic-2-sn-1", "ic-2-sn-2", "ic-2-sn-3",
                               "ic-2-sn-4", "ic-2-sn-5"};
    int departures4[] = {6 * HOURS + 30 * MINUTES, 9 * HOURS + 30 * MINUTES,
                         12 * HOURS + 30 * MINUTES, 15 * HOURS + 30 * MINUTES,
                         18 * HOURS + 30 * MINUTES};
    Vehicle *vehicles4[] = {gir_2, gir_1, gir_2, gir_1, gir_2};

    // Create route
    new_route(network, route_id4, nodes4, arrival_offsets4,
              departure_offsets4,  // Route properties
              route_size3, trip_ids4, departures4, vehicles4,
              trip_size  // Trip properties
    );
    EXPECT_EQ(network->route_counter, 4);

    // Add routes for line IC 6
    const char *route_id5 = "ic-6-ns";
    Node *nodes5[] = {BS, OL, BN, BR};
    int arrival_offsets5[] = {0, 27 * MINUTES, 58 * MINUTES,
                              2 * HOURS + 12 * MINUTES};
    int departure_offsets5[] = {0, 30 * MINUTES, 1 * HOURS + 1 * MINUTES,
                                2 * HOURS + 12 * MINUTES};
    size_t route_size5 = 4;

    // Define trip attributes
    const char *trip_ids5[] = {"ic-6-ns-1", "ic-6-ns-2", "ic-6-ns-3",
                               "ic-6-ns-4", "ic-6-ns-5"};
    int departures5[] = {6 * HOURS + 15 * MINUTES, 9 * HOURS + 15 * MINUTES,
                         12 * HOURS + 15 * MINUTES, 15 * HOURS + 15 * MINUTES,
                         18 * HOURS + 15 * MINUTES};
    Vehicle *vehicles5[] = {ic2_1, ic2_2, ic2_1, ic2_2, ic2_1};

    // Create route
    new_route(network, route_id5, nodes5, arrival_offsets5,
              departure_offsets5,  // Route properties
              route_size5, trip_ids5, departures5, vehicles5,
              trip_size  // Trip properties
    );
    EXPECT_EQ(network->route_counter, 5);

    // Reverse direction
    const char *route_id6 = "ic-6-sn";
    Node *nodes6[] = {BR, BN, OL, BS};
    int arrival_offsets6[] = {0, 1 * HOURS + 6 * MINUTES,
                              1 * HOURS + 45 * MINUTES,
                              2 * HOURS + 12 * MINUTES};
    int departure_offsets6[] = {0, 1 * HOURS + 9 * MINUTES,
                                1 * HOURS + 48 * MINUTES,
                                2 * HOURS + 12 * MINUTES};

    // Define trip attributes
    const char *trip_ids6[] = {"ic-6-sn-1", "ic-6-sn-2", "ic-6-sn-3",
                               "ic-6-sn-4", "ic-6-sn-5"};
    int departures6[] = {6 * HOURS + 15 * MINUTES, 9 * HOURS + 15 * MINUTES,
                         12 * HOURS + 15 * MINUTES, 15 * HOURS + 15 * MINUTES,
                         18 * HOURS + 15 * MINUTES};
    Vehicle *vehicles6[] = {ic2_2, ic2_1, ic2_2, ic2_1, ic2_2};

    // Create route
    new_route(network, route_id6, nodes6, arrival_offsets6,
              departure_offsets6,  // Route properties
              route_size5, trip_ids6, departures6, vehicles6,
              trip_size  // Trip properties
    );
    EXPECT_EQ(network->route_counter, 6);

    // Add routes for line IC 21
    const char *route_id7 = "ic-21-ns";
    Node *nodes7[] = {BS, OL, LZ, GD, LG};
    int arrival_offsets7[] = {0, 25 * MINUTES, 1 * HOURS + 2 * MINUTES,
                              1 * HOURS + 42 * MINUTES,
                              2 * HOURS + 55 * MINUTES};
    int departure_offsets7[] = {0, 28 * MINUTES, 1 * HOURS + 5 * MINUTES,
                                1 * HOURS + 45 * MINUTES,
                                2 * HOURS + 55 * MINUTES};
    size_t route_size7 = 5;

    // Define trip attributes
    const char *trip_ids7[] = {"ic-21-ns-1", "ic-21-ns-2", "ic-21-ns-3",
                               "ic-21-ns-4", "ic-21-ns-5"};
    int departures7[] = {6 * HOURS + 45 * MINUTES, 9 * HOURS + 45 * MINUTES,
                         12 * HOURS + 45 * MINUTES, 15 * HOURS + 45 * MINUTES,
                         18 * HOURS + 45 * MINUTES};
    Vehicle *vehicles7[] = {icn_1, icn_2, icn_1, icn_2, icn_1};

    // Create route
    new_route(network, route_id7, nodes7, arrival_offsets7,
              departure_offsets7,  // Route properties
              route_size7, trip_ids7, departures7, vehicles7,
              trip_size  // Trip properties
    );
    EXPECT_EQ(network->route_counter, 7);

    // Reverse direction
    const char *route_id8 = "ic-21-sn";
    Node *nodes8[] = {LG, GD, LZ, OL, BS};
    int arrival_offsets8[] = {
        0, 1 * HOURS + 10 * MINUTES, 1 * HOURS + 40 * MINUTES,
        2 * HOURS + 27 * MINUTES, 2 * HOURS + 55 * MINUTES};
    int departure_offsets8[] = {
        0, 1 * HOURS + 13 * MINUTES, 1 * HOURS + 43 * MINUTES,
        2 * HOURS + 30 * MINUTES, 2 * HOURS + 55 * MINUTES};

    // Define trip attributes
    const char *trip_ids8[] = {"ic-21-sn-1", "ic-21-sn-2", "ic-21-sn-3",
                               "ic-21-sn-4", "ic-21-sn-5"};
    int departures8[] = {6 * HOURS + 45 * MINUTES, 9 * HOURS + 45 * MINUTES,
                         12 * HOURS + 45 * MINUTES, 15 * HOURS + 45 * MINUTES,
                         18 * HOURS + 45 * MINUTES};
    Vehicle *vehicles8[] = {icn_2, icn_1, icn_2, icn_1, icn_2};

    // Create route
    new_route(network, route_id8, nodes8, arrival_offsets8,
              departure_offsets8,  // Route properties
              route_size7, trip_ids8, departures8, vehicles8,
              trip_size  // Trip properties
    );
    EXPECT_EQ(network->route_counter, 8);

    // Reservation
    // Node *orig = get_node(network, "Zürich HB");
    // Node *dest = get_node(network, "Lugano");
    // Connection *con = new_connection(orig, dest, 60 * 60 * 12);
    // new_reservation(con, 2);
    // new_reservation(con, 1);
    // new_reservation(con->next->next, 5);
    // export_network(network, "intercity2.xml");

    // Free memory
    delete_network(network);
}

/**
 * @brief Test network destruction.
 *
 */
TEST(NetworkTest, Destroy) {
    // Initialize network
    Network *network = new_network();

    // Free memory of all structures in the memory and the network itself
    delete_network(network);

    // valgrind ./network_test
    EXPECT_TRUE(1);
}