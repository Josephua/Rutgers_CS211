#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cofactor(int** matrix, int** temp, int p, int q, int n) {

	int i = 0, j = 0;

	for (int r = 0; r < n; r++) {
		for (int c = 0; c < n; c++) {
			if (r != p && c != q) {
				temp[i][j++] = matrix[r][c];
				if (j == n - 1) {
					j = 0;
					i++;
				}
			}
		}
	}

}

int det(int **matrix, int n) {
	
	int d = 0;

	if (n == 1) {
		return matrix[0][0];
	}

	int **temp = malloc(n * sizeof(int*));
	int i;
	int j;

	for (i = 0; i < n; i++) {
		temp[i] = malloc(n * sizeof(int));
	}

	int sign = 1;

	for (j = 0; j < n; j++) {

		cofactor(matrix, temp, 0, j, n);
		d += sign * matrix[0][j] * det(temp, n - 1);

		sign = -sign;	

	}

	return d;

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

	int n;

	fscanf(fp, "%d\n", &n);

	int **matrix = malloc(n * sizeof(int*));

	int i;
	int j;

	for (i = 0; i < n; i++) {
		matrix[i] = malloc(n * sizeof(int));
	}

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			fscanf(fp, "%d", &matrix[i][j]);
		}
	}

	printf("%d\n", det(matrix, n));

	fclose(fp);
	
	for (int i = 0; i < n; i++) {
		free(matrix[i]);
	}
	
	free(matrix);
	
	return EXIT_SUCCESS;

}

