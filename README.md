![run-tests](../../workflows/run-tests/badge.svg)

## 2020 Project 2

Εκφώνηση: https://k08.chatzi.org/projects/project2/


### Προσωπικά στοιχεία

__Όνομα__: Ντέιβιντ Μπούσι

__Α.Μ.__: sdi1900127


### Documentation

__Άσκηση 1:__
Real time complexity:
![real time](charts/realtime.png)
Παρατηρούμε ότι η real time πολυπλοκότητα είναι Ο(1) πάντα εκτός από όταν διπλασιάζεται το capacity του vector, όπου είναι O(n), επειδή καλείται η realloc, που αντιγράφει τα υπάρχοντα στοιχεία στις νέες τους διευθύνσεις.

Amortized time complexity:
![amortized time](charts/amortized.png)
Το amortized time ξεκινάει από το 1 και πλησιάζει στο 3 και στην συνέχεια ταλαντεύεται μεταξύ του 2 και του 3, χωρίς να ξεπερνάει ποτέ το 3.

Επειδή η χείριστη πολυπλοκότητα Ο(n) παρατηρείται μόνο στον διπλασιασμό του capacity, δηλαδή ελάχιστες φορές, είναι καλύτερο να λάβουμε υπόψιν την amortized time complexity για να μελετήσουμε την πολυπλοκότητα της vector_insert_last. Αφού η τελευταία δεν ξεπερνάει το 3, μπορούμε να πούμε ότι η vector_insert_last έχει γενικά πολυπλοκότητα Ο(1).

__Άσκηση 2:__
Ο τύπος της blist_insert αλλάχθηκε από void σε BListNode, ώστε η συνάρτηση να επιστρέφει τον BListNode που μόλις εισήχθη.

__Άσκηση 3:__
Το ADTSet ήδη χρησιμοποιεί nodes, και επιτρέπει την προσπέλαση των στοιχείων βάση της CompareFunc. Άρα, ένα Priority Queue μπορεί να υλοποιηθεί με ένα ADTSet που επιπλέον επιτρέπει την διαγραφή ενός κόμβου που δεν είναι απαραίτητα ο πρώτος, που είναι και το ζητούμενο της άσκησης.
Με information hiding, το PriorityQueueNode είναι στην ουσία __casted SetNode__ και το PriorityQueue είναι ένα struct που περιέχει το Set με τα στοιχεία.

__Άσκηση 5:__
Γίνεται χρήση του έτοιμου ADTBList από την άσκηση 2.