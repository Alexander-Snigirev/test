#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct Queue {
    Node* front;
    Node* rear;
} Queue;

Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

void enqueue(Queue* queue, int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = NULL;

    if (queue->rear == NULL) {
        new_node->prev = NULL;
        queue->front = new_node;
        queue->rear = new_node;
    } else {
        new_node->prev = queue->rear;
        queue->rear->next = new_node;
        queue->rear = new_node;
    }
}

int dequeue(Queue* queue) {
    if (queue->front == NULL) {
        printf("Queue is empty\n");
        return -1;  // Return -1 if queue is empty
    }

    Node* front_node = queue->front;
    int data = front_node->data;

    if (queue->front == queue->rear) {
        queue->front = NULL;
        queue->rear = NULL;
    } else {
        queue->front = front_node->next;
        queue->front->prev = NULL;
    }

    free(front_node);
    return data;
}

void printQueue(Queue* queue) {
    Node* current = queue->front;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

int main() {
    Queue* queue = createQueue();

    enqueue(queue, 10);
    enqueue(queue, 20);
    enqueue(queue, 30);

    printf("Queue: ");
    printQueue(queue);

    int data1 = dequeue(queue);
    int data2 = dequeue(queue);

    printf("Dequeued elements: %d, %d\n", data1, data2);

    printf("Queue after dequeue: ");
    printQueue(queue);

    return 0;
}
