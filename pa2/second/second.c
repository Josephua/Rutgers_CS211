#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <assert.h>

struct Node** graph;

// Node for Linked List per vertex
  
struct Node { 

	char data[21]; 
	int weight;
	struct Node* next; 

};

// Create and allocates a node
// used by addEdge() method

struct Node* createNode(char a[21], int w) {

	struct Node* newNode = malloc(sizeof(struct Node));
	strcpy(newNode->data, a);
	newNode->weight = w;
	newNode->next = NULL;
	
	return newNode;

}

// Create a list

struct Node* createList(char s[21]) {

	struct Node* list = malloc(sizeof(struct Node));

	strcpy(list->data, s);
	list->next = NULL;

	return list;

}

// Add edge
void addEdge(int v, char a[21], char b[21], int w) {

	// Add edge from a to b

	struct Node* newNode = createNode(b, w);

	for (int i = 0; i < v; i++) {
		if (strcmp(graph[i]->data, a) == 0) {
			if (graph[i] -> next == 0) {
				graph[i] -> next = newNode;
				break;
			}
			else {
				struct Node* temp = graph[i]->next;
				struct Node* prev = 0;
				while (temp != 0 && strcmp(temp->data,b)<0) {
					prev = temp;
					temp = temp -> next;
				}
				if (temp == 0) {
					assert(prev != 0);
					prev -> next = newNode;
				}
				else {
					newNode -> next = temp;
					if (prev == 0) {
						graph[i]->next = newNode;
					}
					else {
						prev -> next = newNode;
					}
				}
			}	
			break;
		}
	}
}

void printGraph(int v) {

	// refers back to main method

	for (int i = 0; i < v; i++) {
		struct Node* ptr = graph[i]->next;
		while (ptr != 0) {
			printf("%s, %d -> ", ptr->data, ptr->weight);
			ptr = ptr -> next;
		}
		printf("\n");
	}

}

int main(int argc, char* argv[argc + 1]) {

	if(argc < 2) {
		printf("insufficient arguments\n");
		return EXIT_SUCCESS;
	}

	FILE* fp = fopen(argv[1], "r");

	if (fp == 0) {
		
		fclose(fp);

		return EXIT_SUCCESS;
	}

	int v = 0;
	
	fscanf(fp, "%d\n", &v);

	// printf("%d\n", v);

	struct Node* l;
	
	char k[21];

	graph = malloc(sizeof(struct Node*)*v);

	for (int i = 0; i < v; i++) {
		fscanf(fp, "%s\n", k);
		// printf("%s\n", k);
		l = createList(k);
		graph[i] = l;
	}

	char a[21];
	char b[21];
	int w;

	while (fscanf(fp, "%s %s %d\n", a, b, &w) != EOF) {
		// Create a node for the first part of addEdge
		// printf("%s %s\n", a, b);
		addEdge(v,a,b,w);
	}

	// printGraph(v);

	FILE* fp2 = fopen(argv[2], "r");

	if (fp2 == 0) {

		fclose(fp);
		fclose(fp2);
		
		return EXIT_SUCCESS;
	}

	char c[21];
	char d[21];

	while (fscanf(fp2, "%s %s\n", c, d) != EOF) {
        	if (strcmp(c,"o") == 0) {
			int count = 0;
			for (int i = 0; i < v; i++) {
				if (strcmp(graph[i]->data, d) == 0) {
					struct Node* temp = graph[i]->next;
					while (temp != 0) {
						temp = temp -> next;
						count++;
					}
				}
			}
            		printf("%d\n", count);
		}
		else if (strcmp(c,"a") == 0) {
			for (int j = 0; j < v; j++) {
				if (strcmp(graph[j]->data, d) == 0) {
					struct Node* temp = graph[j]->next;
					while (temp != 0) {
						printf("%s ", temp->data);
						temp = temp -> next;
					}
				}
			}
            		printf("\n");
		}
		else {
			int count2 = 0;
			for (int i = 0; i < v; i++) {
				struct Node* ptr = graph[i]->next;
				while (ptr != 0) {
					if (strcmp(ptr->data,d) == 0) {
						count2++;
					}
					ptr = ptr -> next;
				}
			}
			printf("%d\n", count2);
		}
	}

	fclose(fp2);
	fclose(fp);
	
}
