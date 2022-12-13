#include <gtest/gtest.h>

extern "C" {
#include <osurs/olal.h>
#include <osurs/io.h>
#include <osurs/reserve.h>
}

TEST(OlalTest, BasicTest) {

	// Initialize network
	Network* network = new_network();
	// Nodes
	Node* n1 = new_node(network, "Albisrieden", 0.0, 0.0);
	Node* n2 = new_node(network, "Buelach", 1.0, 0.0);
	Node* n3 = new_node(network, "Chur", 1.0, 1.0);
	Node* n4 = new_node(network, "Dietikon", 0.0, 1.0);
	// Compositions
	Composition* train = new_composition(network, "train", 10);
	// Vehicles
	Vehicle* v1 = new_vehicle(network, "rt-1", train);
	Vehicle* v2 = new_vehicle(network, "rt-2", train);
	Vehicle* v3 = new_vehicle(network, "rt-3", train);
	Vehicle* v4 = new_vehicle(network, "rt-4", train);
	Vehicle* v5 = new_vehicle(network, "rt-5", train);

	// Define route attributes (closed, circular route)
	const char* route_id = "blue";
	Node* nodes[] = { n1, n2, n3, n4, n1 };
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
	Connection* c1 = new_connection(n1, n3, 12 * HOURS);
	Connection* c2 = new_connection(n1, n2, 12 * HOURS);
	Connection* c3 = new_connection(n2, n4, 12 * HOURS);
	Connection* c4 = new_connection(n3, n4, 12 * HOURS);

	// Add reservations
	Reservation* r1 = new_reservation(c1, 1);
	Reservation* r2 = new_reservation(c2, 1);
	Reservation* r3 = new_reservation(c3, 1);
	Reservation* r4 = new_reservation(c4, 1);

	SeatCollection* result_collection = optimize_trip(c1->trip);

	EXPECT_EQ(result_collection->seat_arr[0]->res_count, 2);
	EXPECT_EQ(result_collection->seat_arr[1]->res_count, 2);
	EXPECT_EQ(result_collection->seat_arr[0]->res_id_arr[0], r1->res_id);
	EXPECT_EQ(result_collection->seat_arr[0]->res_id_arr[1], r4->res_id);
	EXPECT_EQ(result_collection->seat_arr[1]->res_id_arr[0], r2->res_id);
	EXPECT_EQ(result_collection->seat_arr[1]->res_id_arr[1], r3->res_id);

	delete_seat_collection(result_collection);
	delete_connection(c1);
	delete_connection(c2);
	delete_connection(c3);
	delete_connection(c4);
	delete_network(network);
}
