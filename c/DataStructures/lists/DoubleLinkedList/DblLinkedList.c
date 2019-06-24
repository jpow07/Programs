#include <stdio.h>
#include <stdlib.h>

#include "DblLinkedList.h"

void push_back(NodePtr *node, int data) {
	NodePtr newTail = (NodePtr) malloc(sizeof(Node));
	newTail->value = data;
	newTail->next = NULL;

	NodePtr head = *node;

	if (head == NULL) {
		newTail->prev = NULL;
		head = newTail; 
		head->head = newTail;
		head->tail = newTail;
		head->size = 1;
		*node = head;
		return;
	}

	head->size++;

	NodePtr oldTail = head->tail;
	newTail->prev = oldTail;
	oldTail->next = newTail;
	head->tail = newTail;
}

void push_front(NodePtr *node, int data) {
	NodePtr newHead = (NodePtr) malloc(sizeof(Node));
	newHead->value = data;
	newHead->head = newHead;
	newHead->prev = NULL;

	NodePtr oldHead = *node;

	if(oldHead == NULL) {
		newHead->tail = newHead;
		newHead->next = NULL;
		newHead->prev = NULL;
		newHead->size = 1;
		*node = newHead;
		return;
	}

	newHead->size = oldHead->size + 1;
	oldHead->size = 0;
	newHead->tail = oldHead->tail;
	oldHead->head = NULL;
	oldHead->tail = NULL;
	newHead->next = oldHead;
	newHead->prev = NULL;
	oldHead->prev = newHead;	
	
	*node = newHead;

}

void pop_back(NodePtr *node) {
	NodePtr head = *node;
	
	if(head == NULL) {
		printf("%s", "\033[38;2;255;0;0m");
		puts("List has no Nodes to remove.\n");
		printf("%s", "\033[38;2;255;255;255m");
		return;
	} else if (head->next == NULL) {
		*node = NULL;
		free(head);
		return;
	}
	
	head->size--;
	NodePtr oldTail = (*node)->tail;

	head->tail = oldTail->prev;
	oldTail->prev->next = NULL;
	oldTail->prev = NULL;
	free(oldTail);
}

void pop_front(NodePtr *node) {
	NodePtr oldHead = *node;

	if (oldHead == NULL) {
		printf("%s", "\033[38;2;255;0;0m");
		puts("List has no Nodes to remove.\n");
		printf("%s", "\033[38;2;255;255;255m");
		return;
	} else if (oldHead->next == NULL) {
		*node = NULL;
		free(oldHead);
		oldHead = NULL;
		return;
	}

	NodePtr newHead = oldHead->next;
	newHead->size = oldHead->size - 1;
	newHead->head = newHead;
	newHead->tail = oldHead->tail;
	newHead->prev = NULL;
	
	free(oldHead);
	*node = newHead;

}

void printList(NodePtr *node) {
	Node *itr = *node;
	if(itr == NULL) { 
		printf("%s", "\033[38;2;255;0;0m");
		puts("List is empty.\n");
		printf("%s", "\033[38;2;255;255;255m");
	}
	
	while(itr != NULL) {
		if(itr == *node)
			printf("%s", "\033[38;2;0;255;0m");
		else
			printf("%s", "\033[38;2;255;255;255m");

		printf("------------------------------------\n"
			"|Node Address:      %-15p|\n"
			"|Node Value:        %-15d|\n"
			"|Node Head:         %-15p|\n"
			"|Node Tail:         %-15p|\n"
			"|Node Size:         %-15d|\n"
			"|Node Next Address: %-15p|\n"
			"|Node Prev Address: %-15p|\n"
			"------------------------------------\n",
		       	itr, itr->value, itr->head, itr->tail, 
			itr->size, itr->next, itr->prev);
		
		itr = itr->next;
	}
		printf("%s", "\033[38;2;255;255;255m");
}

