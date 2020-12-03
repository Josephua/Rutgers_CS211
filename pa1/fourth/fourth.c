#include <stdio.h> 
#include <stdlib.h> 

int main(int argc, char* argv[argc + 1]) {

	if(argc < 2) {
		printf("insufficient arguments\n");
		return EXIT_SUCCESS;
	}

	int n = atoi(argv[1]);

	if (n % 2 == 0 || n < 1) {
		printf("error\n");
		return EXIT_SUCCESS;
	}  

	int **ms = malloc(n * sizeof(int*));

	for (int i = 0; i < n; i++) {
		ms[i] = malloc(n * sizeof(int));
	}
    
	int count = 1;	

	int i = 0;
	int j = (n/2);
	int down = 1;

	while (count <= (n * n)) {
		
		if (n == 1) {
			ms[0][0] = count;
			break;
		}

		ms[i][j] = count;

		if (i == 0 && j != (n - 1)) {
			i = (n - 1);
			j++;
		}
		else if (j == (n - 1) && i != 0) {
			j = 0;
			i--;
		}
		else if (down == n) {
			i++;
			down = 0;
		}
		else {
			i--;
			j++;
		}
		
		count++;
		down++;
		
	}

	for (int a = 0; a < n; a++) {
		for (int b = 0; b < n; b++) {
			printf("%d\t", ms[a][b]);
			if(b == (n - 1)) {
            			printf("\n");
         		}
		}
	}

	for (int i = 0; i < n; i++) {
		free(ms[i]);
	}

	free(ms);

	return EXIT_SUCCESS; 

}
