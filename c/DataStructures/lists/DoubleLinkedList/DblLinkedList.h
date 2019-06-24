
typedef struct Node {
	int size;
	struct Node *head;
	struct Node *tail;

	int value;
	struct Node *next;
	struct Node *prev;

}Node;

typedef Node* NodePtr;

void push_back(NodePtr *node, int data);
void push_front(NodePtr *node, int data);
void pop_back(NodePtr *node);
void pop_front(NodePtr *node);
void printList(NodePtr *node);
