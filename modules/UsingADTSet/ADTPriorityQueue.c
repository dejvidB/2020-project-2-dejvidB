///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Priority Queue μέσω σωρού.
//
///////////////////////////////////////////////////////////
#include <stdlib.h>
#include <assert.h>

#include "ADTPriorityQueue.h"
#include "ADTSet.h"			// Η υλοποίηση του PriorityQueue χρησιμοποιεί Set
#include "ADTVector.h"

// Ενα PriorityQueue είναι pointer σε αυτό το struct
struct priority_queue {
	Set set;				    // Τα δεδομένα, σε Set
	CompareFunc compare;		// Η διάταξη
	DestroyFunc destroy_value;	// Συνάρτηση που καταστρέφει ένα στοιχείο του vector.
};

// Συναρτήσεις του ADTPriorityQueue //////////////////////////////////////////////////

PriorityQueue pqueue_create(CompareFunc compare, DestroyFunc destroy_value, Vector values) {
	assert(compare != NULL);	// LCOV_EXCL_LINE

	PriorityQueue pqueue = malloc(sizeof(*pqueue));
	pqueue->compare = compare;
	pqueue->destroy_value = destroy_value;

    pqueue->set = set_create(compare, NULL);
	// Αν values != NULL, αρχικοποιούμε το σωρό.
	if (values != NULL)
		for(VectorNode node = vector_first(values); node != VECTOR_EOF; node = vector_next(values, node))
            set_insert(pqueue->set, vector_node_value(values, node));

	return pqueue;
}

int pqueue_size(PriorityQueue pqueue) {
	return set_size(pqueue->set);
}

Pointer pqueue_max(PriorityQueue pqueue) {
	return set_node_value(pqueue->set, set_last(pqueue->set));		// root
}

PriorityQueueNode pqueue_insert(PriorityQueue pqueue, Pointer value) {
	set_insert(pqueue->set, value);	// Εισαγωγή στo set
    return (PriorityQueueNode)set_find_node(pqueue->set, value); // Εύρεση του στοιχείου που προστέθηκε και cast σε PriorityQueueNode
}

void pqueue_remove_max(PriorityQueue pqueue) {
	DestroyFunc old_destroy = set_set_destroy_value(pqueue->set, pqueue->destroy_value);
	set_remove(pqueue->set, set_node_value(pqueue->set, set_last(pqueue->set))); // Αφαίρεση του set_last, που είναι το μέγιστο με βάση την CompareFunc
	set_set_destroy_value(pqueue->set, old_destroy);
}

DestroyFunc pqueue_set_destroy_value(PriorityQueue pqueue, DestroyFunc destroy_value) {
	DestroyFunc old = pqueue->destroy_value;
	pqueue->destroy_value = destroy_value;
	return old;
}

void pqueue_destroy(PriorityQueue pqueue) {
    set_set_destroy_value(pqueue->set, pqueue->destroy_value);
    set_destroy(pqueue->set);
	free(pqueue);
}



//// Νέες συναρτήσεις για την εργασία 2 //////////////////////////////////////////

Pointer pqueue_node_value(PriorityQueue set, PriorityQueueNode node) {
    return set_node_value(set->set, (SetNode)node);
}

void pqueue_remove_node(PriorityQueue pqueue, PriorityQueueNode node) {
	DestroyFunc old_destroy = set_set_destroy_value(pqueue->set, pqueue->destroy_value);
    set_remove(pqueue->set, set_node_value(pqueue->set, (SetNode)node));
	set_set_destroy_value(pqueue->set, old_destroy);
}

void pqueue_update_order(PriorityQueue pqueue, PriorityQueueNode node) {

}