#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

struct Node { 
	int data; 
	struct Node* next; 
};

struct Node* head = NULL;

void pop() {

	if (head == NULL) {
		return;
	}

	head = head -> next;
	return;

}

void push(int d) {

	if (head == NULL) {
		head = malloc(sizeof(struct Node));
		head -> data = d;
		head -> next = NULL;
		return;
	}	

	struct Node* newNode = malloc(sizeof(struct Node));
	newNode -> data = d;
	newNode -> next = head;
	head = newNode;
	return;

}

void enqueue(int d) {

	if (head == NULL) {
		head = malloc(sizeof(struct Node));
		head -> data = d;
		head -> next = NULL;
		return;
	}	

	struct Node* newNode = malloc(sizeof(struct Node));
	newNode -> data = d;
	newNode -> next = NULL;
	
	struct Node* ptr = head;

	while (ptr -> next != NULL) {
		ptr = ptr -> next;
	}

	ptr -> next = newNode;

	return;

}

void printList() {

	if (head == NULL) {
		printf("EMPTY\n");
		return;
	}

	struct Node* ptr = head;

	while (ptr != NULL) { 
		printf("%d ", ptr->data); 
		ptr = ptr->next; 
	} 

	printf("\n");

} 

int main(int argc, char* argv[argc + 1]) {

	if(argc < 2) {
		printf("insufficient arguments\n");
		return EXIT_SUCCESS;
	}

	FILE* fp = fopen(argv[1], "r");

	if (fp == 0) {
		printf("Could not open file\n");
		return EXIT_SUCCESS;
	}
	
	char i[8];
	int j = 0;

	while (fscanf(fp, "%s\n", i) != EOF) {
		if (strcmp("POP", i) == 0) {
			pop();
		}
		else {
			fscanf(fp, "%d\n", &j);
			if (strcmp("PUSH", i) == 0) {
				push(j);
			}
			else {
				enqueue(j);
			}
		}
		printList();
	}

	return EXIT_SUCCESS; 
	
}
