#include <stdio.h>
#include <stdlib.h>

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
	
	int i;

	while (fscanf(fp, "%d\n", &i) != EOF) {
		int isPrime = 1;
		if (i % 2 == 0) {
			isPrime = 0;
		}
		if (i == 1) {
			isPrime = 0;
		}
		if (i < 0) {
			isPrime = 0;
		}
		if (isPrime == 1) {
			for (int a = 3; a * a <= i; a++) {
				if (i % a == 0) {
					isPrime = 0;
				}	
			}
			if (isPrime) {
				int j = i - 2;
				for (int b = 3; b * b <= j; b++) {
					if (j % b == 0) {
						isPrime = 0;
						break;
					}
				}
				if (!isPrime) {
					isPrime = 1;
					int k = i + 2;
					for (int c = 3; c * c <= k; c++) {
						if (k % c == 0) {
							isPrime = 0;
							break;
						}
					}
				}
			}
		}
		if (isPrime == 1) {
			printf("yes\n");
		}
		else {
			printf("no\n");
		}
	}
	
	fclose(fp);

	return EXIT_SUCCESS;

}
