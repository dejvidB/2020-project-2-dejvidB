# Κάνοντας compile το <foo>_test.c με μια υλοποίηση <foo>.c του
# συγκεκριμένου τύπου, παράγουμε ένα tets για την υλοποίηση αυτή.

# Υλοποιήσεις μέσω συνδεδεμένης λίστας: ADTList
#
UsingLinkedList_ADTList_test_OBJS       = ADTList_test.o $(MODULES)/UsingLinkedList/ADTList.o

# Υλοποιήσεις μέσω διπλά συνδεδεμένης λίστας: ADTΒList
#
UsingDoublyLL_ADTBList_test_OBJS        = ADTBList_test.o $(MODULES)/UsingDoublyLL/ADTBList.o

# Υλοποιήσεις μέσω Heap: ADTPriorityQueue
# Το Heap αλλά και το test χρησιμοποιούν Vector, οπότε χρειαζόμαστε και μια υλοποίηση του ADTVector.
#
UsingHeap_ADTPriorityQueue_test_OBJS    = ADTPriorityQueue_test.o $(MODULES)/UsingHeap/ADTPriorityQueue.o $(MODULES)/UsingDynamicArray/ADTVector.o

# Υλοποιήσεις μέσω BinarySearchTree: ADTSet, ADTSet_without_BList
UsingBinarySearchTree_ADTSet_test_OBJS  = ADTSet_test.o $(MODULES)/UsingBinarySearchTree/ADTSet.o $(MODULES)/UsingDoublyLL/ADTBList.o
UsingBinarySearchTree_ADTSet_without_BList_test_OBJS  = ADTSet_test.o $(MODULES)/UsingBinarySearchTree/ADTSet_without_BList.o

# Υλοποιήσεις μέσω Set: ADTPriorityQueue
UsingADTSet_ADTPriorityQueue_test_OBJS  =  ADTPriorityQueue_test.o $(MODULES)/UsingADTSet/ADTPriorityQueue.o $(MODULES)/UsingBinarySearchTree/ADTSet.o $(MODULES)/UsingDynamicArray/ADTVector.o $(MODULES)/UsingDoublyLL/ADTBList.o

# Ο βασικός κορμός του Makefile
include ../common.mk