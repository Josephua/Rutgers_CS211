#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void convertComplement(long a, long b) {
	int* binary = malloc(sizeof(int)*b);
	if (abs(a) >= floor(pow (2,b-1))) { // 1 << (b - 1)
		if (a > 0) {
			printf("0");
			for (int i = b - 1; i > 0; i--) {
				printf("1");
			}
			printf("\n");
		}
		else if (a < 0) {
			printf("1");
			for (int i = b - 1; i > 0; i--) {
				printf("0");
			}
			printf("\n");
		}
		else {
			for (int i = b - 1; i >= 0; i--) {
				printf("0");
			}
			printf("\n");
		}
	}
	else {
		int count = 0;
		for (int i = b - 1; i >= 0; i--) {
			int j = abs(a) >> i;
			if ((j & 1) > 0) {
				binary[count] = 1;
				count++;
			}
			else {
				binary[count] = 0;
				count++;
			}
		}
		int convert = 0;
		if (a < 0) {
			for (int i = b - 1; i >= 0; i--) {
				if (convert == 0 && binary[i] == 1) {
					convert = 1;
					continue;
				}
				if (convert == 1) {
					if (binary[i] == 1) {
						binary[i] = 0;
					}
					else if (binary[i] == 0) {
						binary[i] = 1;
					}
				}
			}
		}
		for (int i = 0; i < b; i++) {
			printf("%d", binary[i]);
		}
		printf("\n");
	}
	free(binary);
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

	long a, b;
	
	while (fscanf(fp, "%ld %ld\n", &a, &b) != EOF) {
		convertComplement(a, b);
	}
	fclose(fp);

	return EXIT_SUCCESS;
}
