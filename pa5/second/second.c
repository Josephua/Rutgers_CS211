#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <stdbool.h>
#include <assert.h>

unsigned long int memReads = 0;
unsigned long int memWrites = 0;
unsigned long int L1misses = 0;
unsigned long int L1hits = 0;
unsigned long int L2misses = 0;
unsigned long int L2hits = 0;

typedef struct CacheLine {
	int valid;
	size_t tag;
	size_t address;
	unsigned long int time;
} CacheLine;

CacheLine** L1cache;
CacheLine** L2cache;

bool isPowerOfTwo(int n) {
	if (n == 0) {
		printf("error\n");
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
	CacheLine** cache;
	cache = malloc(sizeof(CacheLine*) * setNum);
	for (int i = 0; i < setNum; i++) {
		cache[i] = malloc(sizeof(CacheLine) * associativity);
		for (int j = 0; j < associativity; j++) {
			cache[i][j].valid = 0;
			cache[i][j].tag = 0;
			cache[i][j].address = 0;
			cache[i][j].time = 0;
		}
	}
	return cache;
}

void populateL2(unsigned long int blockSet, size_t blockTag, size_t L2address, unsigned long int associativity2, unsigned long int* timer2) {
	// fill in cold cache
	// printf("%ld ", associativity2);
	for (unsigned long int j = 0; j < associativity2; j++) {
		if (L2cache[blockSet][j].valid == 0) {
			L2cache[blockSet][j].tag = blockTag;
			L2cache[blockSet][j].valid = 1;
			L2cache[blockSet][j].address = L2address;
			L2cache[blockSet][j].time = timer2[blockSet];
			timer2[blockSet]++;
			// printf("%ld %ld %zd %d %zd %ld\n", blockSet, j, L2cache[blockSet][j].tag, L2cache[blockSet][j].valid, L2cache[blockSet][j].address, L2cache[blockSet][j].time);
			return;
		}
	}
	// replacement policy
	unsigned long int lowest = ULONG_MAX;
	unsigned long int j = 0;
	for (int i = 0; i < associativity2; i++) {
		if (L2cache[blockSet][i].valid == 1) {
			if (L2cache[blockSet][i].time < lowest) {
				lowest = L2cache[blockSet][i].time;
				j = i;
			}
		}
	}
	L2cache[blockSet][j].address = L2address;
	L2cache[blockSet][j].tag = blockTag;
	L2cache[blockSet][j].time = timer2[blockSet];
	// printf("%zd %d %zd %ld\n", L2cache[blockSet][j].tag, L2cache[blockSet][j].valid, L2cache[blockSet][j].address, L2cache[blockSet][j].time);
	timer2[blockSet]++;
}

void populateL1(unsigned long int setNum2, int blocksize, size_t address, unsigned long int* timer, unsigned long int* timer2, bool inCache, unsigned long int blockSet, size_t blockTag, unsigned long int associativity, unsigned long int associativity2, int i) {
	if (inCache) {
		L1cache[blockSet][i].time = timer[blockSet];
		timer[blockSet]++;
	}
	else {
		// fill in cold cache
		for (unsigned long int j = 0; j < associativity; j++) {
			if (L1cache[blockSet][j].valid == 0) {
				L1cache[blockSet][j].tag = blockTag;
				L1cache[blockSet][j].valid = 1;
				L1cache[blockSet][j].address = address;
				L1cache[blockSet][j].time = timer[blockSet];
				timer[blockSet]++;
				return;
			}
		}
		// replacement policy
		unsigned long int lowest = ULONG_MAX;
		unsigned long int j = 0;
		for (int i = 0; i < associativity; i++) {
			if (L1cache[blockSet][i].valid == 1) {
				if (L1cache[blockSet][i].time < lowest) {
					lowest = L1cache[blockSet][i].time;
					j = i;
				}
			}
		}
		// filling in L2 cache
		unsigned long int blockSet2;
		unsigned long int setBits2;
		unsigned long int offset;
		size_t L2address;
		size_t blockTag2;
		L2address = L1cache[blockSet][j].address;
		offset = log10(blocksize)/log10(2);
		setBits2 = log10(setNum2)/log10(2);
		blockSet2 = (unsigned long int)((L2address >> offset) % (unsigned long int)(pow(2,(double)setBits2)));
		blockTag2 = L2address >> (setBits2 + offset);
		// printf("%zd %zd %ld %ld %ld %ld\n", L1cache[blockSet][j].address, L2address, blockSet2, setBits2, setNum2, offset);
		populateL2(blockSet2, blockTag2, L2address, associativity2, timer2);
		// finalizing replacement policy
		L1cache[blockSet][j].address = address;
		L1cache[blockSet][j].tag = blockTag;
		L1cache[blockSet][j].time = timer[blockSet];
		timer[blockSet]++;
	}
}

void freeL1Cache(unsigned long int setNum) {
	for (int i = 0; i < setNum; i++) {
		free(L1cache[i]);
	}
	free(L1cache);
}

void freeL2Cache(unsigned long int setNum) {
	for (int i = 0; i < setNum; i++) {
		free(L2cache[i]);
	}
	free(L2cache);
}

bool isValid(int L1CacheSize, int L2CacheSize, int blocksize, char* policy1, char* policy2, FILE* fp) {
	//check file
	if (fp == 0) {
		fclose(fp);
		return false;
	}
	// verify powers of 2
	// size of L1 cache
	unsigned long int po2 = isPowerOfTwo(L1CacheSize);
	if (po2 == 0) {
		return false;
	}
	// size of L1 cache
	po2 = isPowerOfTwo(L2CacheSize);
	if (po2 == 0) {
		return false;
	}
	// blocksize
	po2 = isPowerOfTwo(blocksize);
	if (po2 == false) {
		return false;
	}
	// L1 policy
	if (strcmp(policy1,"lru") != 0 && strcmp(policy1,"fifo") != 0) {
		return false;
	}
	// L2 policy
	if (strcmp(policy2,"lru") != 0 && strcmp(policy2,"fifo") != 0) {
		return false;
	}
	// else
	return true;
}

int main(int argc, char* argv[argc + 1]) {

	if(argc < 8) {
		printf("error\n");
		return EXIT_SUCCESS;
	}

	int L1CacheSize = atoi(argv[1]);
	char* assoc1 = argv[2];
	char* policy1 = argv[3];
	int blocksize = atoi(argv[4]);
	int L2CacheSize = atoi(argv[5]);
	char* assoc2 = argv[6];
	char* policy2 = argv[7];
	FILE* fp = fopen(argv[8], "r");
	
	bool validity = isValid(L1CacheSize, L2CacheSize, blocksize, policy1, policy2, fp);
	if (validity == false) {
		printf("error\n");
		return EXIT_SUCCESS;
	}

	// get associativity for L1 cache
	// associativity = number of blocks per set (columns)
	// setNum = number of sets (rows)
	unsigned long int associativity1;
	unsigned long int setNum1;
	if (strcmp(assoc1,"direct") == 0) {
		// sets (rows) with 1 block (column) each
		associativity1 = 1;
		setNum1 = (L1CacheSize/blocksize);
		// printf("Number of sets: %d\n", setNum);
	}
	else if(strcmp(assoc1,"assoc") == 0){
		// 1 set (row) with many blocks (columns)
		associativity1 = L1CacheSize/blocksize;
		setNum1 = 1;
		// printf("Number of sets: %d\n", setNum);
	}
	else {
		// taking associativity (num of columns) and using it to find setNum
		char* val = strtok(assoc1, ":");
		val = strtok(NULL, ":");
		// printf("%s\n", val);
		associativity1 = atoi(val);
		setNum1 = (L1CacheSize/(associativity1 * blocksize));
		// printf("Number of sets: %d\n", setNum1);
	}

	// printf("%s\n", assoc2);
	// get associativity for L2 cache
	unsigned long int associativity2;
	unsigned long int setNum2;
	if (strcmp(assoc2,"direct") == 0) {
		// sets (rows) with 1 block (column) each
		associativity2 = 1;
		setNum2 = (L2CacheSize/blocksize);
		// printf("Number of sets: %d\n", setNum);
	}
	else if(strcmp(assoc2,"assoc") == 0){
		// 1 set (row) with many blocks (columns)
		associativity2 = L2CacheSize/blocksize;
		setNum2 = 1;
		// printf("Number of sets: %d\n", setNum);
	}
	else {
		// taking associativity (num of columns) and using it to find setNum
		char* val = strtok(assoc2, ":");
		val = strtok(NULL, ":");
		// printf("%s\n", val);
		associativity2 = atoi(val);
		setNum2 = (L2CacheSize/(associativity2 * blocksize));
		// printf("Number of sets: %d\n", setNum1);
	}
	// printf("%ld %ld %d ", setNum2, associativity2, L2CacheSize);

	// timer for L1 cache
	unsigned long int* timer = malloc(sizeof(unsigned long int)*setNum1);
	for (int j = 0; j < setNum1; j++) {
		timer[j] = 0;
	}

	// timer for L2 cache
	unsigned long int* timer2 = malloc(sizeof(unsigned long int)*setNum2);
	for (int j = 0; j < setNum2; j++) {
		timer2[j] = 0;
	}

	// create cache
	L1cache = createCache(associativity1, setNum1);
	L2cache = createCache(associativity2, setNum2);

	unsigned long int offset;
	unsigned long int blockSet1;
	unsigned long int setBits1;
	char command;
	size_t address;
	size_t blockTag;

	offset = log10(blocksize)/log10(2);
	setBits1 = log10(setNum1)/log10(2);

	while (fscanf(fp, "%c %lx", &command, &address) != EOF) {
		// blockOffset = address & ((1 << offset) - 1);
		blockSet1 = (unsigned long int)((address >> offset) % (unsigned long int)(pow(2,(double)setBits1)));
		blockTag = address >> (setBits1 + offset);
		bool inL1Cache = false;
		bool inL2Cache = false;
		// printf("%zd %ld %ld %ld %ld\n", address, blockSet1, setBits1, setNum1, offset);
		if (blockSet1 > setNum1) {
			printf("error\n");
			return EXIT_SUCCESS;
		}
		// check if block is inside L1 cache
		for (int i = 0; i < associativity1; i++) {
			if (L1cache[blockSet1][i].tag == blockTag && L1cache[blockSet1][i].valid == 1) {
				inL1Cache = true;
				if (strcmp(policy1,"lru") == 0) {
					populateL1(setNum2, blocksize, address, timer, timer2, inL1Cache, blockSet1, blockTag, associativity1, associativity2, i);
				}
				break;
			}
		}
		// check if block is inside L2 cache
		unsigned long int setBits2;
		unsigned long int blockSet2;
		size_t blockTag2;
		setBits2 = log10(setNum2)/log10(2);
		blockTag2 = address >> (setBits2 + offset);
		blockSet2 = (unsigned long int)((address >> offset) % (unsigned long int)(pow(2,(double)setBits2)));
		// size_t tempa;
		// size_t tempb;
		// int tempc;
		for (int j = 0; j < associativity2; j++) {
			// printf("%zd %zd %d \n", L2cache[i][j].tag, blockTag2, L2cache[i][j].valid);
			if (L2cache[blockSet2][j].tag == blockTag2 && L2cache[blockSet2][j].valid == 1) {
				inL2Cache = true;
				// tempa = L2cache[blockSet2][j].tag;
				// tempb = blockTag2;
				// tempc = L2cache[blockSet2][j].valid;
				L2cache[blockSet2][j].valid = 0;
				L2cache[blockSet2][j].tag = 0;
				L2cache[blockSet2][j].address = 0;
				L2cache[blockSet2][j].time = 0;
				break;
			}
		}
		// read
		// printf("%d\n", inL2Cache);
		if (command == 'R') {
			if (inL1Cache) {
				L1hits++;
			}
			else if (!inL1Cache) {
				L1misses++;
				if (inL2Cache) {
					// printf("%zd %zd %d \n", tempa, tempb, tempc);
					L2hits++;
					populateL1(setNum2, blocksize, address, timer, timer2, inL1Cache, blockSet1, blockTag, associativity1, associativity2, -1);
				}
				else {
					L2misses++;
					memReads++;
					populateL1(setNum2, blocksize, address, timer, timer2, inL1Cache, blockSet1, blockTag, associativity1, associativity2, -1);
				}
			}
		}
		else if (command == 'W') { // write
			memWrites++;
			if (inL1Cache) {
				L1hits++;
			}
			else if (!inL1Cache) {
				L1misses++;
				if (inL2Cache) {
					L2hits++;
					// printf("%zd %zd %d \n", tempa, tempb, tempc);
					populateL1(setNum2, blocksize, address, timer, timer2, inL1Cache, blockSet1, blockTag, associativity1, associativity2, -1);
				}
				else {
					memReads++;
					L2misses++;
					populateL1(setNum2, blocksize, address, timer, timer2, inL1Cache, blockSet1, blockTag, associativity1, associativity2, -1);
				}
			}
		}
	}
	fclose(fp);

	printf("memread:%ld\n", memReads);
	printf("memwrite:%ld\n", memWrites);
	printf("l1cachehit:%ld\n", L1hits);
	printf("l1cachemiss:%ld\n", L1misses);
	printf("l2cachehit:%ld\n", L2hits);
	printf("l2cachemiss:%ld\n", L2misses);

	freeL1Cache(setNum1);
	freeL2Cache(setNum2);
	free(timer);
	free(timer2);

	return EXIT_SUCCESS;

}
