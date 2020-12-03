#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct Node** graph;

// Node for Linked List per vertex

// Graph methods

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

// isVisited

struct visited {
	char name[21];
	int isVisited;
};

void dfsTopSort(struct Node* input, struct visited* arr, int v) {
	for (int i = 0; i < v; i++) {
		if (strcmp(input->data,arr[i].name) == 0 && arr[i].isVisited == 0) {
			printf("%s ", input->data); // print
			arr[i].isVisited = 1;
		}
	}
	while (input != 0) {
		input = input->next; // for each neighbor of input
		for (int j = 0; j < v; j++) {
			if (input != 0 && (strcmp(input->data,arr[j].name) == 0) && arr[j].isVisited == 0) { // if j is not visited
				for (int k = 0; k < v; k++) { // dfs graph[k]
					if (strcmp(graph[k]->data,arr[j].name) == 0) {
						dfs(graph[k], arr, v);
					}
				}
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

	printf("\n");

	int v = 0;
	fscanf(fp, "%d\n", &v);
	// printf("%d\n", v);
	struct Node* l;
	graph = malloc(sizeof(struct Node*)*v);
	struct visited* arr = malloc(sizeof(struct visited)*v);
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
		strcpy(arr[i].name,graph[i]->data);
		arr[i].isVisited = 0;
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
	for (int i = 0; i < v; i++) {
		if (strcmp(graph[i]->data,arr[i].name) == 0) {
			dfs(graph[i], arr, v);
		}
	}

	printf("\n");

	free(arr);

}
