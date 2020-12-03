#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <assert.h>

struct Node** graph;

// queue

// Node for Linked List per vertex
struct Node { 
	char data[21]; 
	struct Node* next; 
	int nodeCount;
};

// Graph functions

// Create and allocates a node
// used by addEdge() method
struct Node* createNode(char a[21]) {
	struct Node* newNode = malloc(sizeof(struct Node));
	strcpy(newNode->data, a);
	newNode->next = 0;
	newNode->nodeCount = 0;
	return newNode;
}

// Create a list
struct Node* createList(char s[21]) {
	struct Node* list = malloc(sizeof(struct Node));
	strcpy(list->data, s);
	list->next = NULL;
	list->nodeCount = 0;
	return list;
}

// Add edge
void addEdge(int v, char a[21], char b[21]) {
	// Add edge from a to b
	struct Node* newNode = createNode(b);
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
	newNode = createNode(a);
	for (int i = 0; i < v; i++) {
		if (strcmp(graph[i]->data, b) == 0) {
			if (graph[i] -> next == 0) {
				graph[i] -> next = newNode;
				break;
			}
			else {
				struct Node* temp = graph[i]->next;
				struct Node* prev = 0;
				while (temp != 0 && strcmp(temp->data,a) < 0) {
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

// bfs methods
// used by dequeue
struct visited {
	char name[21];
	int isVisited;
};

struct queue {
	struct Node* front;
};

struct queue* createQueue() {
	struct queue* q = malloc(sizeof(struct queue));
	q->front = 0;
	return q;
}

void enqueue(char target[21], struct queue* q) {
	struct Node* targetNode = createNode(target);
	
	if (q->front == 0) {
		q->front = targetNode;
		return;
	}
	else if (q->front->next == 0) {
		q->front->next = targetNode;
	}
	else {
		struct Node* ptr = q->front;
		while (ptr->next != 0) {
			ptr = ptr->next;
		}
		ptr->next = targetNode;
	}
	
}

int isEmpty(struct queue* q) {
	if (q->front == 0) {
		return 1;
	}
	return 0;
}

struct Node* dequeue(struct queue* q) {
	if (isEmpty(q)) {
		printf("error\n");
		return EXIT_SUCCESS;
	}
	struct Node* temp = q->front;
	q->front = temp->next;
	return temp;
}

void bfTraversal(char c[21], struct visited* arr, int v, struct queue* q) { 
	enqueue(c, q);
	while (!isEmpty(q)) {
		// printf("%s ", q->front->data);
		struct Node* temp = dequeue(q);
		struct Node* ptr = 0;
		for (int j = 0; j < v; j++) {
			if (strcmp(temp->data,graph[j]->data) == 0) {
				ptr = graph[j]->next;	
				if (arr[j].isVisited == 1) {
					ptr = 0;
				}
				else {
					arr[j].isVisited = 1;
					printf("%s ", temp->data);
				}
				free (temp);
				break;
			}
		}
		while (ptr != 0) {
			// printf("%s\n", ptr->data);
			int index = -1;
			for (int k = 0; k < v; k++) {
				if (strcmp(ptr->data,graph[k]->data) == 0) {
					index = k;
					break;
				}
			}
			if (arr[index].isVisited == 0) {
				enqueue(ptr->data, q);
			}
			ptr = ptr->next;
		}
	}
}

/*
struct cmp {
	char d[21];
	int n;
};
*/

/*
void bfSearch(char c[21], struct visited* arr, int v) { 
	struct queue* q = createQueue();
	bfTraversal(c, arr, v, q);
	struct cmp* str = malloc(sizeof(struct cmp)*v);
	truct cmp t;
	
	for (int i = 0; i < v; i++) {
		int count = 0;
		struct Node* temp = graph[i]->next;
		while (temp != 0) {
			temp = temp -> next;
			count++;
		}
		graph[i]->nodeCount = count;
	}
	
	
	
	for (int i = 0; i < v; i++) {
		strcpy(str[i].d,graph[i]->data);
		str[i].n = graph[i]->nodeCount;
	}
	for(int i = 0; i < v; i++) {
		for(int j = i + 1; j < v; j++) {
			if (str[i].n < str[j].n) {
				t = str[i];
				str[i] = str[j];
				str[j] = t;
			}
			else {
				if (strcmp(str[i].d, str[j].d) > 0) {
					t = str[i];
					str[i] = str[j];
					str[j] = t;
				}
			}
		}
	}
	
	for (int i = 0; i < v; i++) {
		for (int j = 0; j < v; j++) {
			if (strcmp(graph[j]->data,arr[j].name) == 0 && arr[j].isVisited == 0) {
				bfTraversal(graph[i]->data, arr, v, q);
			}
		}
	}
	printf("\n");
	free(q);
	for (int i = 0; i < v; i++) {
		arr[i].isVisited = 0;
	}
}
*/

void bfSearch(char c[21], struct visited* arr, int v) {
	struct queue* q = createQueue();
	bfTraversal(c, arr, v, q);
	printf("\n");
	free(q);
	for (int i = 0; i < v; i++) {
		arr[i].isVisited = 0;
	}
}

void printGraph(int v) {
	// refers back to main method
	for (int i = 0; i < v; i++) {
		struct Node* ptr = graph[i]->next;
		while (ptr != 0) {
			printf("%s -> ", ptr->data);
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

	while (fscanf(fp, "%s %s\n", a, b) != EOF) {
		// Create a node for the first part of addEdge
		// printf("%s %s\n", a, b);
		addEdge(v,a,b);
	}

	// printGraph(v);

	FILE* fp2 = fopen(argv[2], "r");
	if (fp2 == 0) {
		fclose(fp);
		fclose(fp2);		
		return EXIT_SUCCESS;
	}
	char c[21];
	while (fscanf(fp2, "%s\n", c) != EOF) {
		bfSearch(c, arr, v);
	}
	
	free(arr);

	fclose(fp2);
	fclose(fp);	

}
