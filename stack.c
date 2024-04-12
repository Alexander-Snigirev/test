#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct Stack {
    Node* top;
} Stack;

Stack* createStack() {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->top = NULL;
    return stack;
}

int isEmpty(Stack* stack) {
    return stack->top == NULL;
}

void push(Stack* stack, int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = stack->top;
    newNode->prev = NULL;

    if (stack->top != NULL) {
        stack->top->prev = newNode;
    }

    stack->top = newNode;
}

int pop(Stack* stack) {
    if (isEmpty(stack)) {
        printf("Stack is empty\n");
        return -1;
    }
    int data = stack->top->data;
    Node* temp = stack->top;
    stack->top = stack->top->next;

    if (stack->top != NULL) {
        stack->top->prev = NULL;
    }

    free(temp);
    return data;
}

int peek(Stack* stack) {
    if (isEmpty(stack)) {
        printf("Stack is empty\n");
        return -1;
    }
    return stack->top->data;
}

int main() {
    Stack* stack = createStack();

    push(stack, 1);
    push(stack, 2);
    push(stack, 3);

    printf("Top element: %d\n", peek(stack));

    printf("Popped element: %d\n", pop(stack));
    printf("Popped element: %d\n", pop(stack));
    printf("Popped element: %d\n", pop(stack));

    printf("Is stack empty: %d\n", isEmpty(stack));

    return 0;
}
