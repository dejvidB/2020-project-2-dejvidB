///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT BList μέσω διπλά συνδεδεμένης λίστας.
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>

#include "private_ADTBList.h" 	// Περιέχει την δήλωση της συνάρτησης last_inserted_node
#include "ADTBList.h"

// Ενα BList είναι pointer σε αυτό το struct
struct blist {
	BListNode dummy;			// χρησιμοποιούμε dummy κόμβο, ώστε ακόμα και η κενή λίστα να έχει έναν κόμβο.
	BListNode last;				// δείκτης στον τελευταίο κόμβο, ή στον dummy (αν η λίστα είναι κενή)
	int size;					// μέγεθος, ώστε η list_size να είναι Ο(1)
	DestroyFunc destroy_value;	// Συνάρτηση που καταστρέφει ένα στοιχείο της λίστας.
	BListNode last_inserted;	// Ο κόμβος που εισήχθη τελευταίος στην blist
};

struct blist_node {
	BListNode next;		// Δείκτης στον επόμενο
	BListNode previous; // Δείκτης στον προηγούμενο
	Pointer value;		// Η τιμή που αποθηκεύουμε στον κόμβο
};

BListNode blist_last_inserted(BList blist){
	return blist->last_inserted;
}

BList blist_create(DestroyFunc destroy_value) {
	// Πρώτα δημιουργούμε το stuct
	BList blist = malloc(sizeof(*blist));
	blist->size = 0;
	blist->destroy_value = destroy_value;

	// Χρησιμοποιούμε dummy κόμβο, ώστε ακόμα και μια άδεια λίστα να έχει ένα κόμβο
	// (απλοποιεί τους αλγορίθμους). Οπότε πρέπει να τον δημιουργήσουμε.
	//
	blist->dummy = malloc(sizeof(*blist->dummy));
	blist->dummy->next = NULL;		// άδεια λίστα, ο dummy δεν έχει επόμενο
	blist->dummy->previous = NULL;  // και σίγουρα δεν έχει πρηγούμενο
	// Σε μια κενή λίστα, τελευταίος κόμβος είναι επίσης ο dummy
	blist->last = blist->dummy;

	return blist;
}

int blist_size(BList blist) {
	return blist->size;
}

void blist_insert(BList blist, BListNode node, Pointer value) {
	// Δημιουργία του νέου κόμβου
	BListNode new = malloc(sizeof(*new));
	new->value = value;

	if(node == BLIST_EOF){
		if(blist->dummy->next == NULL){ // Αν η λίστα είναι άδεια, τότε ο κόμβος που προστίθεται είναι ο πρώτος
			blist->dummy->next = new;   // Σύνδεση dummy <--> new
			new->previous = blist->dummy;
			new->next = BLIST_EOF;
			blist->last = new;
		}else{							// αλλιώς είναι ο τελευταίος
			new->next = BLIST_EOF;		// Σύνδεση τελευταίου στοιχείου με new
			new->previous = blist->last;
			blist->last->next = new;
			blist->last = new;
		}
	}else{
		// Σύνδεση του new ανάμεσα στο node, το node->next και το node->previous
		new->next = node;
		new->previous = node->previous;

		new->previous->next = new;
		node->previous = new;
	}
	// Ενημέρωση του size
	blist->last_inserted = new;
	blist->size++;
}

void blist_remove(BList blist, BListNode node) {
	assert(node != NULL);

	if (blist->destroy_value != NULL)
		blist->destroy_value(node->value);

	// Σύνδεση του προηγούμενου κόμβου του node με τον επόμενο κόμβο του node
	node->previous->next = node->next;

	if (node->next != BLIST_EOF)  // Αν ο node δεν είναι ο τελευταίος κόμβος
		node->next->previous = node->previous;  // Σύνδεση του επόμενου κόμβου με τον προηγούμενο του node
	else
		blist->last = node->previous; // Ενημέρωση του blist->last με τον νέο τελευταίο κόμβο

	blist->size--; //Ενημέρωση του size
	free(node);
}

Pointer blist_find(BList blist, Pointer value, CompareFunc compare) {
	for(BListNode node = blist_first(blist); node != BLIST_EOF; node = blist_next(blist, node))
		if(compare(value, node->value) == 0)
			return node->value;
	return NULL;
}

DestroyFunc blist_set_destroy_value(BList blist, DestroyFunc destroy_value) {
	DestroyFunc old = blist->destroy_value;
	blist->destroy_value = destroy_value;
	return old;
}

void blist_destroy(BList blist) {
	// Διασχίζουμε όλη τη λίστα και κάνουμε free όλους τους κόμβους,
	// συμπεριλαμβανομένου και του dummy!
	//
	BListNode node = blist->dummy;
	while (node != NULL) {				// while αντί για for, γιατί θέλουμε να διαβάσουμε
		BListNode next = node->next;		// το node->next _πριν_ κάνουμε free!

		// Καλούμε τη destroy_value, αν υπάρχει (προσοχή, όχι στον dummy!)
		if (node != blist->dummy && blist->destroy_value != NULL)
			blist->destroy_value(node->value);

		free(node);
		node = next;
	}

	// Τέλος free το ίδιο το struct
	free(blist);
}

// Διάσχιση της λίστας /////////////////////////////////////////////

BListNode blist_first(BList blist) {
	return blist->dummy->next;
}

BListNode blist_last(BList blist) {
	if (blist->last == blist->dummy)
		return BLIST_EOF;		// κενή λίστα
	else
		return blist->last;
}

BListNode blist_next(BList blist, BListNode node) {
	assert(node != NULL);
	return node->next;
}

BListNode blist_previous(BList blist, BListNode node) {
	assert(node != NULL);
	return node->previous;
}

Pointer blist_node_value(BList blist, BListNode node) {
	assert(node != NULL);
	return node->value;
}

BListNode blist_find_node(BList blist, Pointer value, CompareFunc compare) {
	for(BListNode node = blist_first(blist); node != BLIST_EOF; node = blist_next(blist, node))
		if(compare(value, node->value) == 0)
			return node;
	return NULL;
}