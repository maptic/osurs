#include <gtest/gtest.h>

extern "C" {
#include <osurs/olal.h>
#include <osurs/io.h>
#include <osurs/reserve.h>
}

// First the io tests need to pass

TEST(OlalTest, BasicTest) {

	// initialize network
	Network* network = new_network();
	// nodes
	Node* nodeA = new_node(network, "Albisrieden", 0.0, 0.0);
	Node* nodeB = new_node(network, "Buelach", 1.0, 0.0);
	Node* nodeC = new_node(network, "Chur", 1.0, 1.0);
	Node* nodeD = new_node(network, "Dietikon", 0.0, 1.0);
	// compositions
	Composition* train = new_composition(network, "train", 10);
	// vehicles
	Vehicle* v1 = new_vehicle(network, "rt-1", train);
	Vehicle* v2 = new_vehicle(network, "rt-2", train);
	Vehicle* v3 = new_vehicle(network, "rt-3", train);
	Vehicle* v4 = new_vehicle(network, "rt-4", train);
	Vehicle* v5 = new_vehicle(network, "rt-5", train);
	// Define route attributes (closed, circular route)
	const char* route_id = "blue";
	Node* nodes[] = { nodeA, nodeB, nodeC, nodeD, nodeA };
	int arrival_offsets[] = { 0, 15 * MINUTES, 25 * MINUTES, 40 * MINUTES,
							 60 * MINUTES };
	int departure_offsets[] = { 0, 20 * MINUTES, 30 * MINUTES, 45 * MINUTES,
							   60 * MINUTES };
	size_t route_size = 5;
	// Define trip attributes
	const char* trip_ids[] = { "blue-1", "blue-2", "blue-3", "blue-4", "blue-5" };
	int departures[] = { 6 * HOURS, 9 * HOURS, 12 * HOURS, 15 * HOURS,
						18 * HOURS };
	Vehicle* vehicles[] = { v1, v2, v3, v4, v5 };
	size_t trip_size = 5;
	// Create route
	new_route(network, route_id, nodes, arrival_offsets,
		departure_offsets,  // Route properties
		route_size, trip_ids, departures, vehicles,
		trip_size  // Trip properties
	);

	// Route
	Connection* conn_1 = new_connection(nodeA, nodeC, 12 * HOURS);
	Connection* conn_2 = new_connection(nodeA, nodeB, 12 * HOURS);
	Connection* conn_3 = new_connection(nodeB, nodeD, 12 * HOURS);
	Connection* conn_4 = new_connection(nodeC, nodeD, 12 * HOURS);

	// Add reservations
	Reservation* r1 = new_reservation(conn_1, 1);
	Reservation* r2 = new_reservation(conn_2, 1);
	Reservation* r3 = new_reservation(conn_3, 1);
	Reservation* r4 = new_reservation(conn_4, 1);

	Seat_collection* result_collection = optimize_trip(conn_1->trip);

	EXPECT_EQ(result_collection->seat_arr[0]->res_count, 2);
	EXPECT_EQ(result_collection->seat_arr[1]->res_count, 2);
	EXPECT_EQ(result_collection->seat_arr[0]->res_id_arr[0], r1->res_id);
	EXPECT_EQ(result_collection->seat_arr[0]->res_id_arr[1], r4->res_id);
	EXPECT_EQ(result_collection->seat_arr[1]->res_id_arr[0], r2->res_id);
	EXPECT_EQ(result_collection->seat_arr[1]->res_id_arr[1], r3->res_id);

	delete_seat_collection(result_collection);
}
