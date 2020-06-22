//only for study!!
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
    temp->next = 0; 
    return temp; 
} 

struct Queue* createQueue() 
{ 
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue)); 
    q->front = q->rear = 0; 
    return q; 
} 

void enQueue(struct Queue* q, int k) { 
    // Create a new LL node 
    struct QNode* temp = newNode(k); 
  
    // If queue is empty, then new node is front and rear both 
    if (q->rear == 0) { 
        q->front = q->rear = temp; 
        return; 
    } 
  
    // Add the new node at the end of queue and change rear 
    q->rear->next = temp; 
    q->rear = temp; 
} 

int deQueue(struct Queue* q) { 
    // If queue is empty, return 0. 
    if (q->front == 0) 
        return 0; 
  
    // Store previous front and move front one node ahead 
    struct QNode* temp = q->front; 
    int item = temp->key;

    q->front = q->front->next; 
  
    // If front becomes 0, then change rear also as 0 
    if (q->front == 0) 
        q->rear = 0; 

    free(temp);
    return item;
} 

#endif