#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char* mantO;
char* mantR;
char* expoArr;
char* expoArr2;

void roundUp(int a, int m, int e) {
	// printf("%c\n", mantO[a]);
	// printf("%d\n", e);
	if (mantO[a] == '0') {
		mantO[a] = '1';
		return;
	}
	else if (a - 1 < 0) {
		expoArr2[e - 1] = '1';
		for (int i = 0; i < m; i++) {
			mantO[i] = '0';
		}
		return;
	}
	else if (mantO[a] == '1') {
		mantO[a] = '0';
		a--;
		roundUp(a, m, e);
	}
}

void convertBinary(double e, int expo) {
	long ex = e;
	int i = 0;
	while (ex > 0) {
		double temp = ex % 2;
		if (temp == 0) {
			expoArr[i] = '0';
		}
		else {
			expoArr[i] = '1';
		}
		ex = ex / 2;
		i++;
	}
	if (i < expo) {
		for (int j = i; j < expo; j++) {
			expoArr[j] = '0';
			i++;
		}
	}
	// printf("%d\n", i);
	// printf("%d\n", expo);
	/*
	for (int k = 0; k < expo; k++) {
		printf("%c", expoArr[k]);
	}
	printf("\n");
	*/
	int temp2 = i;
	for (int j = 0; j < i; j++) { // reversing order
		expoArr2[temp2 - 1] = expoArr[j];
		temp2--;
	}
	/*
	for (int k = 0; k < expo; k++) {
		printf("%c", expoArr[k]);
	}
	printf("\n");
	*/
}

void fractionalBinary(double mantissa, int m, int e) {
	int i = 30;
	int j = 0;
	while (i > 0) {
		mantissa = mantissa * 2;
		if (mantissa >= 1) {
			mantO[j] = '1';
			mantissa -= 1;
		}
		else {
			mantO[j] = '0';
		}
		i--;
		j++;
	}
	/*
	for (int k = 0; k < m + 10; k++) {
		printf("%c", mantO[k]);
	}
	*/
	// printf("\n");
	int a = m - 1;
	int visited = 0;
	for (int c = m; c < m + 30; c++) {
		if (mantO[m] == '0') {
			break;
		}
		if (mantO[c] == '1' && c != m) {
			roundUp(a, m, e);
			visited = 1;
			break;
		}
	}
	if (mantO[m-1] == '1' && mantO[m] == '1' && visited == 0) {
		roundUp(a, m, e);
	}
	for (int k = 0; k < m; k++) {
		mantR[k] = mantO[k];
	}
	/*
	for (int k = 0; k < m; k++) {
		printf("%c", mantR[k]);
	}
	printf("\n");
	*/
}

void convertIEEE(double input, int size, int expo, int mant) {
	char* ieee = malloc(sizeof(char)*size);
	mantO = malloc(sizeof(char)*(mant+30));
	mantR = malloc(sizeof(char)*(mant));
	int c = 0;
	if (input >= 0) {
		ieee[c] = '0';
	}
	else {
		ieee[c] = '1';
	}
	double temp = fabs(input);
	// printf("%lf\n", temp);
	double exponent = 0;
	while (floor(temp) != 1) {
		if (temp > 1) {
			temp = temp/2;
			exponent++;
			// printf("%lf\n", temp);
		}
		else {
			temp = temp * 2;
			exponent--;
			// printf("%lf\n", temp);
			// printf("%lf\n", exponent);
		}
	}
	double mantissa = temp - floor(temp);
	double bias = pow(2,(expo-1)) - 1;
	// printf("%lf\n", temp);
	// printf("%lf\n", exponent);
	double e = 0;
	if (bias + exponent == 0) {
		temp = temp/2;
		mantissa = temp;
	}
	else {
		e = exponent + bias;
	}
	// printf("%lf\n", mantissa);
	// printf("%lf\n", e);
	// printf("%d\n", expo);
	expoArr = malloc(sizeof(char)*99);
	expoArr2 = malloc(sizeof(char)*expo);
	convertBinary(e, expo);
	fractionalBinary(mantissa, mant, expo);
	c++;
	for (int k = 0; k < expo; k++) {
		ieee[c] = expoArr2[k];
		c++;
	}
	// printf("%d\n", c);
	// printf("%d\n", size);
	for (int k = 0; k < mant; k++) {
		ieee[c] = mantR[k];
		c++;
	}
	for (int m = 0; m < size; m++) {
		printf("%c",ieee[m]);
	}
	free(ieee);
	free(mantO);
	free(mantR);
	free(expoArr);
	free(expoArr2);
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
	int b, c, d;

	while (fscanf(fp, "%lf %d %d %d", &a, &b, &c, &d) != EOF) {
		convertIEEE(a, b, c, d);
		printf("\n");
	}
	fclose(fp);

	return EXIT_SUCCESS;
}
