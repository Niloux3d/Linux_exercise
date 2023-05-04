#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct node {
	int element;
	struct node* next;
} Node;

typedef struct queue {
	Node* front;
	Node* rear;
	int size;
	int capacity;
} Queue;

Queue* create_queue(int capacity) {
	Queue* q = (Queue*)malloc(sizeof(Queue));
	q->front = NULL;
	q->rear = NULL;
	q->size = 0;
	q->capacity = capacity;
	return q;
}

bool is_empty(Queue* q) {
	return q->size == 0;
}

bool is_full(Queue* q) {
	return q->size == q->capacity;
}

void push_queue(Queue* q, int val) {
    Node* newNode = (Node*)calloc(1, sizeof(Node));
    newNode->element = val;
    if (q->size == 0){
        q->front = newNode;
        q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    ++q->size;
}

int pop_queue(Queue* q) {
	if (is_empty(q)) {
		printf("Error: queue is empty\n");
		exit(1);
	}
	Node* temp = q->front;
	int retValue = temp->element;
	q->front = temp->next;
	free(temp);
	q->size--;
	return retValue;
}

int peek_queue(Queue* q) {
	if (is_empty(q)) {
		printf("Error: queue is empty\n");
		exit(1);
	}
	Node* peekNode = q->front;
	return peekNode->element;
}

void visit(Queue* q){
    Node* curr = q->front;
    while (curr){
        printf("%d ", curr->element);
        curr = curr->next;
    }
    printf("\n");
}
