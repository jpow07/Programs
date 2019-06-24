#include <stdio.h>
#include <stdlib.h>

#include "DblLinkedList.h"

int main(int argc, char *argv[]) {

	int value = 0;
	NodePtr list = NULL;

	for(int i = 0; i < 9; i++)
		push_front(&list, value++);	
	printList(&list);

	for(int i = 0; i < 9; i++)
		push_back(&list, value++);	
	printList(&list);

	for(int i = 0; i < 9; i++)
		pop_front(&list);	
	printList(&list);

	for(int i = 0; i < 8; i++)
		pop_back(&list);	
	printList(&list);

	pop_front(&list);	
	printList(&list);
	return 0;
}

