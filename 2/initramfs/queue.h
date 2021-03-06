#ifndef QUEUE_STRUCT
#define QUEUE_STRUCT

#include <stdio.h>
#include <stdlib.h>

struct QNode { 
    int key; 
    struct QNode* next; 
}; 

struct Queue { 
    struct QNode *front, *rear; 
}; 

struct QNode* newNode(int k) 
{ 
    struct QNode* temp = (struct QNode*)malloc(sizeof(struct QNode)); 
    temp->key = k; 
    temp->next = NULL; 
    return temp; 
} 

struct Queue* createQueue() 
{ 
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue)); 
    q->front = q->rear = NULL; 
    return q; 
} 

void enQueue(struct Queue* q, int k) { 
    // Create a new LL node 
    struct QNode* temp = newNode(k); 
  
    // If queue is empty, then new node is front and rear both 
    if (q->rear == NULL) { 
        q->front = q->rear = temp; 
        return; 
    } 
  
    // Add the new node at the end of queue and change rear 
    q->rear->next = temp; 
    q->rear = temp; 
} 

int deQueue(struct Queue* q) { 
    // If queue is empty, return NULL. 
    if (q->front == NULL) 
        return 0; 
  
    // Store previous front and move front one node ahead 
    struct QNode* temp = q->front; 
    int item = temp->key;

    q->front = q->front->next; 
  
    // If front becomes NULL, then change rear also as NULL 
    if (q->front == NULL) 
        q->rear = NULL; 

    free(temp);
    return item;
} 

#endif