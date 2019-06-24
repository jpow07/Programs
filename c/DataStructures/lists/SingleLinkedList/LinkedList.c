#include "LinkedList.h"

void push_back(NodePtr *node, int data) {
	NodePtr newNode = (NodePtr) malloc(sizeof(Node));
	newNode->value = data;
	newNode->next = NULL;	

	NodePtr itr = *node;
	if (itr == NULL) {
		*node = newNode;
		return;
	}
	
	while(itr->next != NULL)
		itr = itr->next;

	itr->next = newNode;

}

void printList(NodePtr *node) {
	Node *itr = *node;
	if(itr == NULL) 
		puts("List is empty.\n");

	while(itr != NULL) {
		printf("------------------------------------\n"
			"|Node Address:      %-15p|\n"
			"|Node Value:        %-15d|\n"
			"|Node Next Address: %-15p|\n"
			"------------------------------------\n",
		       	itr, itr->value, itr->next);
		
		itr = itr->next;
	}

}

void pop_back(NodePtr *node) {
	NodePtr itr = *node;

	// If passed a null node	
	if (itr == NULL )
		return;

	// If only the head node remains
	else if (itr->next == NULL) {
		free(itr);
		*node = NULL;
		itr = NULL;
		return;
	}
	// Traverse to end of list and delete last node
	while (itr->next->next != NULL)
		itr = itr->next;
	
	free(itr->next);
	itr->next = NULL;
}
