#include <stdio.h>
#include <stdlib.h>
#include "CircularList.h"

typedef struct Node {

	int value;

	struct Node *first;
	struct Node *next;
} Node;

typedef Node* NodePtr;

void insert(NodePtr *node, int data);
void delete(NodePtr *node);

int main(int argc, char *argv[]) {


	return 0;
}
