typedef struct Node {
    struct Node *prev;
    struct Node *next;
    int value;
} Node;

Node *push(Node *place, int value);
int pop(Node *node);
