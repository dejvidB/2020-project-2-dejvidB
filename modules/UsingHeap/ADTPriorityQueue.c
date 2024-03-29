///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Priority Queue μέσω σωρού.
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>

#include "ADTPriorityQueue.h"
#include "ADTVector.h"			// Η υλοποίηση του PriorityQueue χρησιμοποιεί Vector

// Ενα PriorityQueue είναι pointer σε αυτό το struct
struct priority_queue {
	Vector vector;				// Τα δεδομένα, σε Vector ώστε να έχουμε μεταβλητό μέγεθος χωρίς κόπο
	CompareFunc compare;		// Η διάταξη
	DestroyFunc destroy_value;	// Συνάρτηση που καταστρέφει ένα στοιχείο του vector.
	int steps_counter;			// Μετρητής βημάτων συναρτήσεων
};

// Ενα PriorityQueueNode είναι pointer σε αυτό το struct
struct priority_queue_node {
	Pointer value;
	int position; // H θέση του node στο δέντρο, η οποία ανανεώνεται σε κάθε node_swap.
};

// Βοηθητικές συναρτήσεις ////////////////////////////////////////////////////////////////////////////

// Προσοχή: στην αναπαράσταση ενός complete binary tree με πίνακα, είναι βολικό τα ids των κόμβων να
// ξεκινάνε από το 1 (ρίζα), το οποίο απλοποιεί τις φόρμουλες για εύρεση πατέρα/παιδιών. Οι θέσεις
// ενός vector όμως ξεκινάνε από το 0. Θα μπορούσαμε απλά να αφήσουμε μία θέση κενή, αλλά δεν είναι ανάγκη,
// μπορούμε απλά να αφαιρούμε 1 όταν διαβάζουμε/γράφουμε στο vector. Για απλοποίηση του κώδικα, η
// πρόσβαση στα στοιχεία του vector γίνεται από τις παρακάτω 2 βοηθητικές συναρτήσεις.

// Επιστρέφει την τιμή του κόμβου node_id

static Pointer node_value(PriorityQueue pqueue, int node_id) {
	// τα node_ids είναι 1-based, το node_id αποθηκεύεται στη θέση node_id - 1
	return ((PriorityQueueNode)vector_get_at(pqueue->vector, node_id - 1))->value;
}

// Επιστρέφει το PriorityQueueNode στην θέση node_id
static PriorityQueueNode pqnode(PriorityQueue pqueue, int node_id) {
	return (PriorityQueueNode)vector_get_at(pqueue->vector, node_id - 1);
}

// Ανταλλάσει τους κόμβους node_id1 και node_id2

static void node_swap(PriorityQueue pqueue, int node_id1, int node_id2) {
	// τα node_ids είναι 1-based, το node_id αποθηκεύεται στη θέση node_id - 1
	PriorityQueueNode node1 = pqnode(pqueue, node_id1);
	PriorityQueueNode node2 = pqnode(pqueue, node_id2);

	// Ενημέρωση θέσεων nodes
	int pos1 = node1->position;
	
	node1->position = node2->position;
	node2->position = pos1;

	vector_set_at(pqueue->vector, node_id1 - 1, node2);
	vector_set_at(pqueue->vector, node_id2 - 1, node1);
}

// Αποκαθιστά την ιδιότητα του σωρού.
// Πριν: όλοι οι κόμβοι ικανοποιούν την ιδιότητα του σωρού, εκτός από
//       τον node_id που μπορεί να είναι _μεγαλύτερος_ από τον πατέρα του.
// Μετά: όλοι οι κόμβοι ικανοποιούν την ιδιότητα του σωρού.

static void bubble_up(PriorityQueue pqueue, int node_id) {
	// Αν φτάσαμε στη ρίζα, σταματάμε
	if (node_id == 1)
		return;

	int parent = node_id / 2;		// Ο πατέρας του κόμβου. Τα node_ids είναι 1-based

	// Αν ο πατέρας έχει μικρότερη τιμή από τον κόμβο, swap και συνεχίζουμε αναδρομικά προς τα πάνω
	if (pqueue->compare(node_value(pqueue, parent), node_value(pqueue, node_id)) < 0) {
		node_swap(pqueue, parent, node_id);
		bubble_up(pqueue, parent);
		pqueue->steps_counter++; // Αυξάνουμε τον μετρητή, όσες φορές γίνεται bubble up
	}
}

// Αποκαθιστά την ιδιότητα του σωρού.
// Πριν: όλοι οι κόμβοι ικανοποιούν την ιδιότητα του σωρού, εκτός από τον
//       node_id που μπορεί να είναι _μικρότερος_ από κάποιο από τα παιδιά του.
// Μετά: όλοι οι κόμβοι ικανοποιούν την ιδιότητα του σωρού.

static void bubble_down(PriorityQueue pqueue, int node_id) {
	// βρίσκουμε τα παιδιά του κόμβου (αν δεν υπάρχουν σταματάμε)
	int left_child = 2 * node_id;
	int right_child = left_child + 1;
	int old = pqueue->steps_counter; // Κρατάμε τα βήματα γιατί το pqueue_size αρχικοποιεί τον μετρητή
	int size = pqueue_size(pqueue);
	pqueue->steps_counter = old;
	if (left_child > size)
		return;

	// βρίσκουμε το μέγιστο από τα 2 παιδιά
	int max_child = left_child;
	if (right_child <= size && pqueue->compare(node_value(pqueue, left_child), node_value(pqueue, right_child)) < 0)
			max_child = right_child;

	// Αν ο κόμβος είναι μικρότερος από το μέγιστο παιδί, swap και συνεχίζουμε προς τα κάτω
	if (pqueue->compare(node_value(pqueue, node_id), node_value(pqueue, max_child)) < 0) {
		node_swap(pqueue, node_id, max_child);
		bubble_down(pqueue, max_child);
		pqueue->steps_counter++; // Αυξάνουμε τον μετρητή όσες φορές γίνεται bubble_down
	}
}

static void efficient_heapify(PriorityQueue pqueue, Vector values){
	int pq_size = pqueue_size(pqueue);
	for (int i = 0; i < vector_size(values); i++){
		PriorityQueueNode node = malloc(sizeof(PriorityQueueNode));
		node->value = vector_get_at(values, i);
		node->position = pq_size + 1;
		vector_insert_last(pqueue->vector, node);
		pqueue->steps_counter += vector_steps(pqueue->vector);
	}
	for(int i = pq_size / 2; i >= 1; i--){
		bubble_down(pqueue, i);
	}
	pqueue->steps_counter += vector_size(values) / 2;
}

// Συναρτήσεις του ADTPriorityQueue //////////////////////////////////////////////////

PriorityQueue pqueue_create(CompareFunc compare, DestroyFunc destroy_value, Vector values) {
	assert(compare != NULL);	// LCOV_EXCL_LINE

	PriorityQueue pqueue = malloc(sizeof(*pqueue));
	pqueue->compare = compare;
	pqueue->destroy_value = destroy_value;

	// Δημιουργία του vector που αποθηκεύει τα στοιχεία.
	// ΠΡΟΣΟΧΗ: ΔΕΝ περνάμε την destroy_value στο vector!
	// Αν την περάσουμε θα καλείται όταν κάνουμε swap 2 στοιχεία, το οποίο δεν το επιθυμούμε.
	pqueue->vector = vector_create(0, NULL);
	pqueue->steps_counter = 1; // Αρχικοποίηση μετρητή
	// Αν values != NULL, αρχικοποιούμε το σωρό.
	if (values != NULL)
		efficient_heapify(pqueue, values);

	return pqueue;
}

int pqueue_size(PriorityQueue pqueue) {
	pqueue->steps_counter = 1;
	return vector_size(pqueue->vector);
}

Pointer pqueue_max(PriorityQueue pqueue) {
	pqueue->steps_counter = 1;
	return node_value(pqueue, 1);		// root
}

PriorityQueueNode pqueue_insert(PriorityQueue pqueue, Pointer value) {
	PriorityQueueNode node = malloc(sizeof(PriorityQueueNode));
	node->value = value;
	node->position = pqueue_size(pqueue) + 1;
	// Προσθέτουμε την τιμή στο τέλος το σωρού
	vector_insert_last(pqueue->vector, node);
	pqueue->steps_counter = vector_steps(pqueue->vector);
 	// Ολοι οι κόμβοι ικανοποιούν την ιδιότητα του σωρού εκτός από τον τελευταίο, που μπορεί να είναι
	// μεγαλύτερος από τον πατέρα του. Αρα μπορούμε να επαναφέρουμε την ιδιότητα του σωρού καλώντας
	// τη bubble_up γα τον τελευταίο κόμβο (του οποίου το 1-based id ισούται με το νέο μέγεθος του σωρού).
	bubble_up(pqueue, pqueue_size(pqueue));

	return node;
}

void pqueue_remove_max(PriorityQueue pqueue) {
	int last_node = pqueue_size(pqueue);
	assert(last_node != 0);		// LCOV_EXCL_LINE

	// Destroy την τιμή που αφαιρείται
	if (pqueue->destroy_value != NULL)
		pqueue->destroy_value(pqueue_max(pqueue));

	// Αντικαθιστούμε τον πρώτο κόμβο με τον τελευταίο και αφαιρούμε τον τελευταίο
	node_swap(pqueue, 1, last_node);
	vector_set_destroy_value(pqueue->vector, free);
	vector_remove_last(pqueue->vector);
	vector_set_destroy_value(pqueue->vector, NULL);
	pqueue->steps_counter = 1;
 	// Ολοι οι κόμβοι ικανοποιούν την ιδιότητα του σωρού εκτός από τη νέα ρίζα
 	// που μπορεί να είναι μικρότερη από κάποιο παιδί της. Αρα μπορούμε να
 	// επαναφέρουμε την ιδιότητα του σωρού καλώντας τη bubble_down για τη ρίζα.
	bubble_down(pqueue, 1);
}

DestroyFunc pqueue_set_destroy_value(PriorityQueue pqueue, DestroyFunc destroy_value) {
	pqueue->steps_counter = 1;
	DestroyFunc old = pqueue->destroy_value;
	pqueue->destroy_value = destroy_value;
	return old;
}

void pqueue_destroy(PriorityQueue pqueue) {
	pqueue->steps_counter = 1;
	VectorNode temp = vector_first(pqueue->vector);
	while(temp != VECTOR_EOF){
			VectorNode node = temp;
			temp = vector_next(pqueue->vector, temp);
			if(pqueue->destroy_value != NULL)
				pqueue->destroy_value(((PriorityQueueNode)vector_node_value(pqueue->vector, node))->value);
			free(vector_node_value(pqueue->vector, node));
			pqueue->steps_counter++;
	}
	vector_set_destroy_value(pqueue->vector, NULL);
	vector_destroy(pqueue->vector);
	free(pqueue);
}


//// Νέες συναρτήσεις για την εργασία 2 //////////////////////////////////////////

Pointer pqueue_node_value(PriorityQueue set, PriorityQueueNode node) {
	set->steps_counter = 1;
	return node->value;
}

void pqueue_remove_node(PriorityQueue pqueue, PriorityQueueNode node) {
	int pos = node->position;
	node_swap(pqueue, pos, pqueue_size(pqueue));  // Ανταλλαγή τελευταίου κόμβου με τον node
	pqueue->destroy_value(node->value);
	free(node);
	vector_remove_last(pqueue->vector); // Αφαίρεση node που πλέον βρίσκεται στο τέλος
	pqueue->steps_counter = 1;
	if(pqueue_size(pqueue))  // Αν η λίστα περιέχει στοιχεία
		pqueue_update_order(pqueue, pqnode(pqueue, pos)); // Update_order στον προηγούμενο παλιό κόμβο που ανταλλάχθηκε
}

void pqueue_update_order(PriorityQueue pqueue, PriorityQueueNode node) {
	pqueue->steps_counter = 1;
	// Αν τιμή του κόμβου είναι μεγαλύτερη απ την τιμή του parent, bubble_up, αλλιώς bubble_down
	if(node->position > 1 && node_value(pqueue, node->position) > node_value(pqueue, node->position / 2))
		bubble_up(pqueue, node->position);
	else
		bubble_down(pqueue, node->position);
}

int pqueue_steps(PriorityQueue pqueue){
	return pqueue->steps_counter;
}
