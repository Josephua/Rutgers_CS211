#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <stdbool.h>
#include <assert.h>

unsigned long int memReads = 0;
unsigned long int memWrites = 0;
unsigned long int misses = 0;
unsigned long int hits = 0;
unsigned long int validNum;

typedef struct CacheLine {
	int valid;
	size_t tag;
	unsigned long int time;
} CacheLine;

CacheLine** cache;

bool isPowerOfTwo(int n) {
	if (n == 0) {
		printf("error");
		return EXIT_SUCCESS;
	}
	else if (ceil(log2(n)) == floor(log2(n))) {
		return true;
	}
	else {
		return false;
	}
}

CacheLine** createCache(unsigned long int associativity, unsigned long int setNum) {
	// associativity = number of columns
	// setNum = number of rows
	cache = malloc(sizeof(CacheLine*) * setNum);
	for (int i = 0; i < setNum; i++) {
		cache[i] = malloc(sizeof(CacheLine) * associativity);
		for (int j = 0; j < associativity; j++) {
			cache[i][j].valid = 0;
			cache[i][j].tag = 0;
			cache[i][j].time = 0;
		}
	}
	return cache;
}

void replacement_policy(unsigned long int* timer, unsigned long int blockSet, size_t blockTag, unsigned long int associativity) {
	unsigned long int lowest = ULONG_MAX;
	unsigned long int j = 0;
	for (int i = 0; i < associativity; i++) {
		if (cache[blockSet][i].valid == 1) {
			if (cache[blockSet][i].time < lowest) {
				lowest = cache[blockSet][i].time;
				j = i;
			}
		}
	}
	cache[blockSet][j].tag = blockTag;
	cache[blockSet][j].time = timer[blockSet];
	timer[blockSet]++;
}

void populate(unsigned long int* timer, bool inCache, unsigned long int blockSet, size_t blockTag, unsigned long int associativity, int i) {
	// check for the largest times
	// unsigned long int largestTime = 0;
	// unsigned long int largestTimeIndex = 0;
	/*
	for (unsigned long int j = 0; j < associativity; j++) {
		if (cache[blockSet][j].valid == 1) {
			if (cache[blockSet][j].time > largestTime) {
				largestTime = cache[blockSet][j].time;
				largestTimeIndex = j;
			}
		}
	}
	*/
	// printf("%ld\n", largestTime);
	if (inCache) {
		for (int j = 0; j < associativity; j++) {
			if (cache[blockSet][j].valid == 1) {
				validNum++;
			}
		}
		// largestTime = largestTime % associativity;
		cache[blockSet][i].time = timer[blockSet];
		timer[blockSet]++;
		// cache[blockSet][i].time += associativity;
	}
	else {
		// fill in cold cache
		// largestTime = largestTime % associativity;
		for (unsigned long int j = 0; j < associativity; j++) {
			if (cache[blockSet][j].valid == 0) {
				cache[blockSet][j].tag = blockTag;
				cache[blockSet][j].valid = 1;
				cache[blockSet][j].time = timer[blockSet];
				timer[blockSet]++;
				return;
			}
		}
		replacement_policy(timer, blockSet, blockTag, associativity);
	}
}

void freeCache(CacheLine** cache, unsigned long int setNum) {
	for (int i = 0; i < setNum; i++) {
		free(cache[i]);
	}
	free(cache);
}

bool isValid(int CacheSize, int blocksize, FILE* fp) {
	//check file
	if (fp == 0) {
		fclose(fp);
		return false;
	}
	// verify powers of 2
	unsigned long int po2 = isPowerOfTwo(CacheSize);
	if (po2 == 0) {
		return false;
	}
	po2 = isPowerOfTwo(blocksize);
	if (po2 == false) {
		return false;
	}
	// else
	return true;
}

int main(int argc, char* argv[argc + 1]) {

	if(argc < 6) {
		printf("error");
		return EXIT_SUCCESS;
	}

	int CacheSize = atoi(argv[1]);
	char* assoc = argv[2];
	char* policy = argv[3];
	int blocksize = atoi(argv[4]);
	FILE* fp = fopen(argv[5], "r");
	
	bool validity = isValid(CacheSize, blocksize, fp);
	if (validity == false) {
		printf("error");
		return EXIT_SUCCESS;
	}

	// get associativity
	// associativity = number of blocks per set (columns)
	// setNum = number of sets (rows)
	unsigned long int associativity;
	unsigned long int setNum;
	if (strcmp(assoc,"direct") == 0) {
		// sets (rows) with 1 block (column) each
		associativity = 1;
		setNum = (CacheSize/blocksize);
		// printf("Number of sets: %d\n", setNum);
	}
	else if(strcmp(assoc,"assoc") == 0){
		// 1 set (row) with many blocks (columns)
		associativity = CacheSize/blocksize;
		setNum = 1;
		// printf("Number of sets: %d\n", setNum);
	}
	else {
		// taking associativity (num of columns) and using it to find setNum
		char* val = strtok(assoc, ":");
		val = strtok(NULL, ":");
		// printf("%s\n", val);
		associativity = atoi(val);
		setNum = (CacheSize/(associativity * blocksize));
		// printf("Number of sets: %d\n", setNum);
	}

	unsigned long int* timer = malloc(sizeof(unsigned long int)*setNum);	
	for (int j = 0; j < setNum; j++) {
		timer[j] = 0;
	}

	// create cache
	cache = createCache(associativity, setNum);

	unsigned long int offset;
	unsigned long int blockSet;
	unsigned long int setBits;
	char command;
	size_t address;
	size_t blockTag;

	offset = log10(blocksize)/log10(2);
	setBits = log10(setNum)/log10(2);

	while (fscanf(fp, "%c %lx", &command, &address) != EOF) {
		// blockOffset = address & ((1 << offset) - 1);
		validNum = 0;
		blockSet = (unsigned long int)((address >> offset) % (unsigned long int)(pow(2,(double)setBits)));
		blockTag = address >> (setBits + offset);
		bool inCache = false;
		if (blockSet > setNum) {
			printf("error");
			return EXIT_SUCCESS;
		}
		for (int i = 0; i < associativity; i++) {
			if (cache[blockSet][i].tag == blockTag && cache[blockSet][i].valid == 1) {
				inCache = true;
				// hits++;
				if (strcmp(policy,"lru") == 0) {
					populate(timer, inCache, blockSet, blockTag, associativity, i);
				}
				break;
			}
		}
		if (command == 'R') {
			if (inCache) {
				hits++;
			}
			else if (!inCache) {
				misses++;
				memReads++;
				populate(timer, inCache, blockSet, blockTag, associativity, -1);
			}
		}
		else if (command == 'W') {
			if (inCache) {
				memWrites++;
				hits++;
			}
			else {
				misses++;
				memReads++;
				memWrites++;
				populate(timer, inCache, blockSet, blockTag, associativity, -1);
			}
		}
	}
	fclose(fp);

	printf("memread:%ld\n", memReads);
	printf("memwrite:%ld\n", memWrites);
	printf("cachehit:%ld\n", hits);
	printf("cachemiss:%ld\n", misses);

	freeCache(cache, setNum);
	free(timer);

	return EXIT_SUCCESS;

}
