#include <stdio.h>
#include <stdlib.h>

void convertBinary(long a, long b) {

	for (int i = b - 1; i >= 0; i--) {
		int j = a >> i;
		// printf("%d\n", j);
		// printf("%ld\n", a);
		// printf("%d\n", i);
		if ((j & 1) > 0) {
			printf("1");
		}
		else {
			printf("0");
		}
	}
	printf("\n");
	
}

int main(int argc, char* argv[argc + 1]) {

	if(argc < 2) {
		return EXIT_SUCCESS;
	}
	FILE* fp = fopen(argv[1], "r");
	if (fp == 0) {
		free(fp);
	}

	long a, b;
	
	while (fscanf(fp, "%ld %ld\n", &a, &b) != EOF) {
		convertBinary(a, b);
	}

}

	

	
	
