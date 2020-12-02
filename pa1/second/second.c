#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
  
struct Node { 
	int data; 
	struct Node* next; 
};

struct Node* head = NULL;

void insert(int d) {

	if (head == NULL) {
		head = malloc(sizeof(struct Node));
		head -> data = d;
		head -> next = NULL;
		return;
	}

	struct Node* ptr = head;
	struct Node* prev = NULL;
	
	while (ptr != NULL) {
		if (ptr -> data > d) {
			break;
		}
		if (ptr -> data == d) {
			return;
		}
		prev = ptr;
		ptr = ptr -> next;
	}

	struct Node* newNode = malloc(sizeof(struct Node));
	newNode -> data = d;
	newNode -> next = NULL;

	if (ptr == head) {
		newNode -> next = head;
		head = newNode;
	} 
	else {
		prev -> next = newNode;
		newNode -> next = ptr;
	}

}

void delete(int d) {

	struct Node* ptr = head;
	struct Node* prev = NULL;

	if (head == NULL) {
		return;
	}

	if (head -> data == d) {
		struct Node* temp = head;
		head = head -> next;
		free(temp);
		return;
	}
	
	while (ptr != NULL) {
		if (ptr -> data == d) {
			prev -> next = ptr -> next;
			free(ptr);
			break;
		}
		prev = ptr;
		ptr = ptr -> next;
	}

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

	char i[7];
	int j = 0;

	if (fp == 0) {
		printf("Could not open file\n");
		return EXIT_SUCCESS;
	}

	while (fscanf(fp, "%s %d\n", i, &j) != EOF) {
		if (strcmp("INSERT", i) == 0) {
			insert(j);
		}
		else if (strcmp("DELETE", i) == 0) {
			delete(j);
		}
		printList();
	}
  
	return EXIT_SUCCESS;

} 
