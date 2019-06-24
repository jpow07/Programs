/*
 * The purpose of this Header file is to declare a Single Linked List
 */

#include <stdlib.h>
#include <stdio.h>

typedef struct Node {
	int value;
	struct Node *next;
} Node;

typedef Node* NodePtr;

void push_back(NodePtr *node, int data);
void pop_back(NodePtr *node);
void printList(NodePtr *node);

