#include <ADTBList.h>

// Επιστρέφει τον κόμβο που εισήχθη τελευταίος στην λίστα
// Μη ορισμένη συμπεριφορά αν υπάρξει μεταβολή αυτού του κόμβου, πριν την κλήση της (πχ. blist_remove)
// Καλείται με ασφάλεια ακριβώς μετά την blist_insert
BListNode blist_last_inserted(BList blist);