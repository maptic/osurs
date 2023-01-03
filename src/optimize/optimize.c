/**
 * @brief Seat reservation optimization.
 * @file optimize.c
 * @date: 2022-08-22
 * @author: Tobias Meier
 */

#include "osurs/optimize.h"
#include "osurs/ds.h"
#include <math.h>

 // Public definitions

 // Add a new reservation to a seat
void seat_add_reservation(Seat* seat, Reservation* res) {
	seat->res_arr[seat->res_count] = res;
	seat->res_count++;
}

// Remove all reservations of a seat
void seat_remove_reservations(Seat* seat) {
	for (int i = 0; i < seat->res_count; ++i) {
		seat->res_arr[i] = NULL;
	}
	seat->res_count = 0;
}

// add new seat to a reservation
void reservation_add_seat(Reservation* res, Seat* seat) {
	res->seat_arr[res->seat_arr_index] = seat;
	res->seat_arr_index++;
}

// Remove all seats of a reservation
void reservations_remove_seats(Reservation* res) {
	for (int i = 0; i < res->seat_arr_index; ++i) {
		res->seat_arr[i] = NULL;
	}
	res->seat_arr_index = 0;
}

int space_available(unsigned int log_res_arr[], int res_count, int segment_count,
	unsigned int seat_count, unsigned int new_res) {
	// iterate over each segment
	for (int i = 0; i < segment_count; ++i) {
		// sum all reservations for the specific segment
		unsigned int sum = 0;
		for (int j = 0; j < res_count; ++j) {
			sum += (log_res_arr[j] & ((unsigned int)1 << i)) >> i;
		}
		sum += (new_res & ((unsigned int)1 << i)) >> i;
		// if the sum exceeds the available space return 0
		if (sum > seat_count) {
			return 0;
		}
	}
	return 1;
}

// tree node parameter struct
typedef struct {
	int m;
	double d;
}tree_node_params;

// breadth first tree creation
// processes the first element of the given queue and adds the child nodes to the queue
void create_tree_node(Queue* node_param_queue, Queue* seat_queue, LinkedList* seat_list,
	ListNode** seat_list_node_arr) {
	// dequeue the first element
	tree_node_params* np = ((tree_node_params*)queue_dequeue(node_param_queue));
	// enqueue the seat of the list to the queue
	queue_enqueue(seat_queue, (void*)seat_list_node_arr[np->m]);

	// add the child nodes to the queue
	// if the distance to the next childe nodes equals to zero the leaves of the tree are reached
	if (np->d > 0) {
		// add the child node on the lower end
		tree_node_params* np_l = (tree_node_params*)malloc(sizeof(tree_node_params));
		np_l->m = (int)(floor((double)np->m - np->d));
		np_l->d = floor(np->d) / 2;
		queue_enqueue(node_param_queue, (void*)np_l);
		// add the child node on the upper end
		tree_node_params* np_u = (tree_node_params*)malloc(sizeof(tree_node_params));
		np_u->m = (int)(ceil((double)np->m + np->d));
		np_u->d = floor(np->d) / 2;
		queue_enqueue(node_param_queue, (void*)np_u);
	}
	free(np);
}

void optimize_reservation(unsigned int log_res_arr[], int res_arr_count,
	Reservation** res_arr, int segment_count,
	Seat** seats, int seat_count, order_method method) {
	// parameter check
	if (res_arr_count <= 0) {
		return;
	}

	// datatype size check
	if (segment_count > sizeof(unsigned int) * 8) {
		return;
	}

	// clear all persisting reservations on the given seats of the composition and vice versa.
	for (int i = 0; i < seat_count; ++i) {
		seat_remove_reservations(seats[i]);
	}
	for (int i = 0; i < res_arr_count; ++i) {
		reservations_remove_seats(res_arr[i]);
	}

	// unordered seat array
	Seat** unordered_seats;
	if (method != fill) {
		unordered_seats = (Seat**)malloc(sizeof(Seat*) * seat_count);
		for (int i = 0; i < seat_count; ++i) {
			unordered_seats[i] = new_seat(0);
		}
	}

	// iterate over each seat
	for (int i = 0; i < seat_count; ++i) {
		unsigned int current_res_config = 0;

		// iterate over each reservation
		for (int j = 0; j < res_arr_count; ++j) {
			//
			if (log_res_arr[j] != 0) {
				// break if a seat is fully booked (over all segments)
				// fully booked = logical representation of all ones
				// all ones = 2^segment_count - 1
				if (current_res_config == ((1 << segment_count) - 1)) {
					break;
				}
				// add the reservation to the current seat if there is space
				// bitwise AND equals to 0 if there is no overlap
				if ((current_res_config & log_res_arr[j]) == 0) {
					// add the logical representation of the current reservation
					// to the current seat representation (bitwise OR)
					current_res_config |= log_res_arr[j];
					// add the reservation id to the seat
					if (method == fill) {
						seat_add_reservation(seats[i], res_arr[j]);
						reservation_add_seat(res_arr[j], seats[i]);
					}
					else {
						seat_add_reservation(unordered_seats[i], res_arr[j]);
					}
					// remove the reservation from the array
					log_res_arr[j] = 0;
				}
			}
		}
	}


	switch (method) {
	case fill:
		break;

	case spatial_uniform:;
		// distribute the reserved seats spatial uniform over the seats given in the composition
		// ignore the last seat if it is an even amount of seats
		int seat_count_odd = (seat_count % 2 == 0) ? seat_count - 1 : seat_count;
		// last seat index
		int upper_bound = seat_count_odd - 1;
		// root node parameters
		tree_node_params* tp = (tree_node_params*)malloc(sizeof(tree_node_params));
		// center position of root node
		tp->m = (int)(upper_bound / 2);
		// distance to next child node
		tp->d = (double)tp->m / 2;
		// node counter
		int counter = 0;
		// node processing queue
		Queue* n_q = queue_create();
		// enqueue the root element
		queue_enqueue(n_q, (void*)tp);
		// seat queue
		Queue* seat_queue = queue_create();
		// linked list of seats (spatially linked)
		LinkedList* seat_list = linked_list_create();
		// array of list nodes to access individual ListNode element by index
		ListNode** seat_list_node_arr = (ListNode**)malloc(sizeof(ListNode*) * seat_count_odd);
		// fill the seats into the list and the array
		for (int i = 0; i < seat_count_odd; ++i) {
			linked_list_add_last(seat_list, (void*)seats[i]);
			seat_list_node_arr[i] = seat_list->tail;
		}
		// breadth first tree creation
		// iterate over all nodes (could also be done recursive but due to the fact that the 
		// number of nodes is already known an iterative approach is the better solution)
		for (int i = 0; i < seat_count_odd; ++i) {
			create_tree_node(n_q, seat_queue, seat_list, seat_list_node_arr);
		}
		// the parameter queue as well as the listNode array is no longer needed
		queue_free(n_q);
		free(seat_list_node_arr);

		// cycle over all unsorted seats and place them on the prepared composition seat given by the seat_queue
		int dir = 1; // append direction
		ListNode* next_seat; // next neighbor seat
		for (int i = 0; i < seat_count_odd; ++i) {
			int same_res = 0;
			if (i > 0) {
				// check if one of the current reservations also reserved the last seat
				for (int j = 0; j < unordered_seats[i]->res_count; ++j) {
					if (same_res) {
						break;
					}
					for (int k = 0; k < unordered_seats[i - 1]->res_count; ++k) {
						if (unordered_seats[i]->res_arr[j]->id == unordered_seats[i - 1]->res_arr[k]->id) {
							same_res = 1;
							break;
						}
					}
				}
			}

			Seat* selected_seat;
			if (!same_res) {
				// dequeue the next element from the seat queue
				ListNode* nls = ((ListNode*)queue_dequeue(seat_queue));
				selected_seat = nls->data;
				// determine the next neighbor seat
				if (nls->next != NULL) {
					next_seat = nls->next;
					dir = 1;
				}
				else {
					next_seat = nls->prev;
					dir = -1;
				}
				// remove the seat from the linked list
				linked_list_remove_node(seat_list, nls);
			}
			else {
				// use the previous determined neighbor seat
				ListNode* nls = next_seat;
				selected_seat = nls->data;
				// change the direction
				dir *= -1;
				// determine the next neighbor seat
				if ((((dir < 0)) && (nls->prev != NULL)) || ((dir > 0) && (nls->next == NULL))) {
					next_seat = nls->prev;
				}
				else {
					next_seat = nls->next;
				}

				// remove the seat from the queue and the linked list
				linked_list_remove_node(seat_list, nls);
				queue_remove_node_by_value(seat_queue, (void*)nls);
			}

			// add the reservation to the seat and vice versa
			for (int j = 0; j < unordered_seats[i]->res_count; ++j) {

				seat_add_reservation(selected_seat, unordered_seats[i]->res_arr[j]);
				reservation_add_seat(unordered_seats[i]->res_arr[j], selected_seat);
			}
		}
		queue_free(seat_queue);
		linked_list_clear(seat_list);
		linked_list_free(seat_list);

		// if it was an even seat count add the last seat to the last position of the array
		if (seat_count % 2 == 0) {
			for (int i = 0; i < unordered_seats[seat_count - 1]->res_count; ++i) {
				seat_add_reservation(seats[seat_count - 1], unordered_seats[seat_count - 1]->res_arr[i]);
				reservation_add_seat(unordered_seats[seat_count - 1]->res_arr[i], seats[seat_count - 1]);
			}
		}

		// free the unordered seat array
		for (int i = 0; i < seat_count; ++i) {
			free(unordered_seats[i]);
		}
		free(unordered_seats);
		break;

	default:
		break;
	}
}

