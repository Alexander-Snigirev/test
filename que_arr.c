#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

typedef struct Queue {
    int items[MAX_SIZE];
    int front;
    int rear;
} Queue;

Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = -1;
    queue->rear = -1;
    return queue;
}

int isEmpty(Queue* queue) {
    return (queue->front == -1);
}

int isFull(Queue* queue) {
    return ((queue->rear + 1) % MAX_SIZE == queue->front);
}

void enqueue(Queue* queue, int data) {
    if (isFull(queue)) {
        printf("Queue is full\n");
        return;
    }

    if (isEmpty(queue)) {
        queue->front = 0;
        queue->rear = 0;
    } else {
        queue->rear = (queue->rear + 1) % MAX_SIZE;
    }

    queue->items[queue->rear] = data;
}

int dequeue(Queue* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty\n");
        return -1;  // Return -1 if queue is empty
    }

    int data = queue->items[queue->front];

    if (queue->front == queue->rear) {
        queue->front = -1;
        queue->rear = -1;
    } else {
        queue->front = (queue->front + 1) % MAX_SIZE;
    }

    return data;
}

void printQueue(Queue* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty\n");
        return;
    }

    int i = queue->front;
    while (i != queue->rear) {
        printf("%d ", queue->items[i]);
        i = (i + 1) % MAX_SIZE;
    }
    printf("%d\n", queue->items[i]);
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
