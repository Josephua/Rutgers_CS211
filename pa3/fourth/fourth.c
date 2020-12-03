#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void bf (double a, long b) {
	char* binaryOne = malloc(sizeof(char)*99);
	char* binaryTwo = malloc(sizeof(char)*(b+3));
	char* binaryThree = malloc(sizeof(char)*99);
	long n = floor(a);
	int i = 0;
	// printf("%ld\n", i);
	if (n >= 1) {
		while (n > 0) {
			long temp = n % 2;
			if (temp == 0) {
				binaryOne[i] = '0';
			}
			else {
				binaryOne[i] = '1';
			}
			n = n / 2;
			i++;
		}
		// printf("%d\n", i);
		int temp2 = i;
		for (int j = 0; j < i; j++) { // reversing order
			binaryThree[temp2 - 1] = binaryOne[j];
			temp2--;
		}
	}
	else {
		binaryThree[i] = '0';
		i++;
	}
	binaryThree[i] = '.';
	/*
	printf("%d\n", i);
	printf("%ld\n", b);
	for (int k = 0; k < i + 1; k++) {
		printf("%c", binaryThree[k]);
	}
	printf("\n");
	*/
	int placement = 0;
	for (int k = 0; k < i + 1; k++) {
		if (binaryThree[k] == '.') {
			placement = k;
		}
	}
	int exponent = 0;
	// printf("%d\n", placement);
	while (placement > (b + 2)) {
		for (int k = 0; k < i + 1; k++) {
			if (binaryThree[k] == '.') {
				char temp3 = binaryThree[k - 1];
				binaryThree[k - 1] = binaryThree[k];
				binaryThree[k] = temp3;
				exponent++;
				i--;
				placement--;
				break;
			}
		}
	}
	/*
	for (int k = 0; k < i + 1; k++) {
		printf("%c", binaryThree[k]);
	}
	printf("\n");
	printf("%d\n", i);
	printf("%ld\n", b + 1);
	*/
	for (int k = 0; k < b + 3; k++) {
		binaryTwo[k] = binaryThree[k];
	}
	/*
	for (int k = 0; k < b + 3; k++) {
		printf("%c", binaryTwo[k]);
	}
	*/
	if ((i + 1) != (b + 3)) {
		i++;
		double o = a - floor(a);
		while (i < b + 3) {
			o *= 2;
			if (floor(o) == 1) {
				binaryTwo[i] = '1';
				o -= 1;
			}
			else {
				binaryTwo[i] = '0';
			}
			i++;
		}
	}
	/*
	for (int k = 0; k < b + 3; k++) {
		printf("%c", binaryTwo[k]);
	}
	*/
	if (binaryTwo[0] == '1') {
		while (binaryTwo[1] != '.') {
			for (int k = 0; k < b + 3; k++) {
				if (binaryTwo[k] == '.') {
					char temp3 = binaryTwo[k - 1];
					binaryTwo[k - 1] = binaryTwo[k];
					binaryTwo[k] = temp3;
					exponent++;
					break;
				}
			}
		}
	}
	else {
		while (binaryTwo[0] != '1') {
			char temp4 = binaryTwo[0];
			for (int k = 2; k < b + 3; k++) {
				if (k == 2) {
					binaryTwo[0] = binaryTwo[k];
				}
				else {
					binaryTwo[k - 1] = binaryTwo[k];
				}
			}
			binaryTwo[b + 2] = temp4;
			exponent--;
		}
	}
	for (int k = 0; k < b + 2; k++) {
		printf("%c", binaryTwo[k]);
	}
	printf(" %d", exponent);
	free(binaryOne);
	free(binaryTwo);
	free(binaryThree);
}

int main(int argc, char* argv[argc + 1]) {

	if(argc < 2) {
		return EXIT_SUCCESS;
	}
	FILE* fp = fopen(argv[1], "r");
	if (fp == 0) {
		fclose(fp);
		return EXIT_SUCCESS;
	}

	double a;
	long b;

	while (fscanf(fp, "%lf %lu", &a, &b) != EOF) {
		bf(a, b);
		printf("\n");
	}
	fclose(fp);

	return EXIT_SUCCESS;
}
