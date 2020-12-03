#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <assert.h>
#define INFINITY 9999

struct Node** graph;
int gcount;

// Node for Linked List per vertex
  
struct Node { 

	char data[21]; 
	int weight;
	int mapping;
	struct Node* next; 

};

// Create and allocates a node
// used by addEdge() method

struct Node* createNode(char a[21], int w, int v) {
	struct Node* newNode = malloc(sizeof(struct Node));
	strcpy(newNode->data, a);
	newNode->weight = w;
	newNode->next = NULL;
	for (int i = 0; i < v; i++) {
		if (strcmp(graph[i]->data,newNode->data) == 0) {
			newNode->mapping = graph[i]->mapping;
		}
	}
	return newNode;
}

// Create a list

struct Node* createList(char s[21]) {
	struct Node* list = malloc(sizeof(struct Node));
	strcpy(list->data, s);
	list->next = NULL;
	list->mapping = gcount;
	// printf("%d\n", gcount);
	gcount++;
	return list;
}

// Add edge
void addEdge(int v, char a[21], char b[21], int w) {
	// Add edge from a to b
	struct Node* newNode = createNode(b, w, v);
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

void convert(int** matrix, int v) {
	for (int i = 0; i < v; i++) {
		for (int j = 0; j < v; j++) {
			matrix[i][j] = 0;
		}
	}
	for (int i = 0; i < v; i++) {
		struct Node* ptr = graph[i]->next;
		while (ptr != 0) {
			matrix[i][ptr->mapping] = ptr->weight;
			ptr = ptr -> next;
		}
	}
}

void printMatrix(int** matrix, int v) {

	for (int i = 0; i < v; i++) {
		for (int j = 0; j < v; j++) {
			printf("%d\t", matrix[i][j]);
		}
		printf("\n");
	}

}

void dijkstra(int** matrix, int v, int startnode) {

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (matrix[i][j] == 0) {
				cost[i][j] = INFINITY;
			}
			else {
				cost[i][j] = matrix[i][j];
			}
		}
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
	graph = malloc(sizeof(struct Node*)*v);
	char str[v][21], temp[21];

	for (int i = 0; i < v; i++) {
		fscanf(fp, "%s\n", str[i]);
		// printf("%s\n", k);
		
	}
	for (int i = 0; i < v - 1; i++) { // lexicographical string vertices
		for (int j = 0; j < v - 1; j++) {
			if (strcmp(str[j], str[j + 1]) > 0) {
				strcpy(temp, str[j]);
				strcpy(str[j], str[j+1]);
				strcpy(str[j+1], temp);
			}
		}
	}
	for (int i = 0; i < v; i++) {
		l = createList(str[i]);
		graph[i] = l;
	}

	char a[21];
	char b[21];
	int w;

	int** matrix = malloc(sizeof(int*)*v);

	for (int i = 0; i < v; i++) {
		matrix[i] = malloc(v * sizeof(int));
	}

	while (fscanf(fp, "%s %s %d\n", a, b, &w) != EOF) {
		// Create a node for the first part of addEdge
		// printf("%s %s\n", a, b);
		addEdge(v,a,b,w);
	}

	// printGraph(v);

	FILE* fp2 = fopen(argv[2], "r");

	convert(matrix, v);
	// printMatrix(matrix, v);

	char c[21];

	while (fscanf(fp2, "%s\n", c) != EOF) {
		int n = 0;
		for (int i = 0; i < v; i++) {
			if (strcmp(graph[i]->data,c) == 0) {
				n = graph[i]->mapping;
			}
		}
		dijkstra(matrix, v, n);
	}

	if (fp2 == 0) {
		fclose(fp);
		fclose(fp2);
		return EXIT_SUCCESS;
	}

	fclose(fp2);
	fclose(fp);
	
}
