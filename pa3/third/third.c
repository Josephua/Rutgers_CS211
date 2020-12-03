#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void convertUnsigned (long a, long b) {
	int* binary = malloc(sizeof(int)*b);
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
        int result = 0;
        int base = pow(2, (b - 1));
	int negative = 0;
        for (int i = 0; i < b; i++) {
		if (negative == 0) {
			result += binary[i] * base * -1;
			base = base/2;
			negative = 1;
		}
		else {
			result += binary[i] * base;
			base = base/2;
		}
        }
	printf("%d\n", result);
	free(binary);
}

void convertSigned (long a, long b) {
	int* binary = malloc(sizeof(int)*b);
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
        int result = 0;
        int base = pow(2, (b - 1));
        for (int i = 0; i < b; i++) {
		result += binary[i] * base;
		base = base/2;
        }
	printf("%d\n", result);
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
	char c[21], d[21];
	
	while (fscanf(fp, "%ld %ld %s %s\n", &a, &b, c, d) != EOF) {
		if (strcmp(c, "u") == 0 && strcmp(d, "s") == 0) {
			convertUnsigned(a, b);
		}
		else if (strcmp(c, "s") == 0 && strcmp(d, "u") == 0) {
			convertSigned(a, b);
		}
	}
	fclose(fp);

	return EXIT_SUCCESS;
}
