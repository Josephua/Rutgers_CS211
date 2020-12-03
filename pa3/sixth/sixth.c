#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double value;
int signedBit;
int expo;
int* binaryOne;
int* binaryTwo;

void convertHex (int size, int e, int m, int h) {
	int a = 1; //signed bit
	signedBit = h & (a << (size - 1));
	signedBit = signedBit >> (size - 1);
	expo = (((1 << e) - 1) << m) & h;
	expo = expo >> m;
	// printf("%d\n", expo);
	int mant = ((1 << m) - 1) & h;
	// printf("%d\n", mant);
	int i = 0;
	while (mant > 0 && i < m) {
		int temp = mant % 2;
		if (temp == 0) {
			binaryOne[i] = 0;
		}
		else {
			binaryOne[i] = 1;
		}
		mant = mant / 2;
		// printf("%d\n", i);
		// printf("%d\n", m);
		i++;
	}
	/*
	for (int n = 0; n < m; n++) {
		printf("%d", binaryOne[n]);
	}
	*/
	int temp2 = m - 1;
	for (int j = 0; j < m; j++) { // reversing order
		binaryTwo[temp2] = binaryOne[j];
		temp2--;
	}
	/*
	for (int k = 0; k < m; k++) {
		printf("%d", binaryTwo[k]);
	}
	printf("\n");
	*/
	/*
	while (mant > 0) {
		int temp = mant % 2;
		if (temp == 0) {
			binaryThree[k] = '0';
		}
		else {
			binaryThree[k] = '1';
		}
		temp = temp / 2;
		k++;
	}
	*/
}

void convertDecimal (int size, int ex, int ma, int he, int precision) {
	binaryOne = malloc(sizeof(int)*(ma + 1));
	binaryTwo = malloc(sizeof(int)*(ma + 1));
	// printf("%x\n", hex);
	convertHex(size, ex, ma, he);
	double bias = (pow(2,ex-1) - 1);
	value = 0;
	double e = 0;
	if (expo == 0) {
		e = 1 - bias;
	}
	else {
		e = expo - bias;
	}
	double value2 = 1.0;
	double two = 2.0;
	if (expo == 0) {
		value2 = 0.0;
		for (int i = 0; i < ma; i++) {
			if (binaryTwo[i] == 1) {
				value2 += (1 / two);
			}
			two *= 2;
		}
	}
	else {
		for (int i = 0; i < ma; i++) {
			if (binaryTwo[i] == 1) {
				value2 += (1 / two);
			}
			two *= 2;
		}
	}
	e = pow(2,e);
	value += e;
	value *= value2;
	value *= pow(-1,signedBit);
	printf("%.*lf", precision, value);
	free(binaryOne);
	free(binaryTwo);
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

	int a, b, c, d, e;

	while (fscanf(fp, "%d %d %d %x %d", &a, &b, &c, &d, &e) != EOF) {
		convertDecimal(a, b, c, d, e);
		printf("\n");
	}
	fclose(fp);

	return EXIT_SUCCESS;
}
