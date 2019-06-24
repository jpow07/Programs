#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"


int main(int argc, char *argv[]) {
	
	int value = 0;
	NodePtr list = NULL;
	printList(&list);
	for (int i = 0; i < 7; value++, i++)
		push_back(&list, value);

	puts("PushBack List");
	printList(&list);

	for (int i = 0; i < 3; i++)
		pop_back(&list);

	puts("PopBack List (Partial)");
	printList(&list);

	for (int i = 0; i < 4; i++)
		pop_back(&list);
	
	puts("PopBack List (Remaining)");
	printList(&list);

	pop_back(&list);
	puts("PopBack List (On Empty)");
	printList(&list);

	return 0;
}

