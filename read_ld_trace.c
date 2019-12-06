/*The following program takes the name of the compressed trace file as
input and prints out the first 10 load  effective addresses. The trace
in hex file contains 1 entry per line. This entry is the difference
between the effective address of this load and the effective address of
the previous one. The effective address is assumed to be 0 at the
beginning(i.e. the first line is the actual effective address of the first
load in the trace).*/

#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char str[33]; // 32비트
char str1[33]; // tag
char str2[33]; // index

// 캐시 링크드 리스트
typedef struct Cache {
	int vaild;
	int tag;
}Cache;

// 10진수 정수를 32비트 2진수로 변환하는 함수
void bin(unsigned n)
{
	int i = 0;
	unsigned value = 1 << 31;
	for (i = 0; i < 32; i++)
	{
		str[i] = n / value + 48;

		n %= value;
		value = value >> 1;
	}
}

void main(int argc, char *argv[]) {

	FILE * f;
	
	char fname[50]; // file name
	long diff;
	unsigned int ea = 0;
	int i, j = 0, k = 0; // for문 index
	int blockindex = 0;
	int size = 1024; // cache size
	int block_size = 32; // block size
	int index = 0;
	int hit = 0; // hit
	int miss = 0; // miss
	int result = 0;
	int cache_index = 0;
	int tag;
	struct Cache cache[256][256];
	
	if (argc == 2) {
		/*If there is a command line argument, use it as the gzip
		file name */
		sprintf(fname, "%s", argv[1]);
	} 
	else {
		/*If there are no command line arguments, ask for one and exit */
		printf("Usage : cache-sim.exe program_name\n");
		exit(-1);
	} 

	f = fopen(fname, "r");
	if (!f) {
		/*If the given file does not exist, tell the user and exit*/
		printf("Need to create %s first.\n", argv[1]);
		exit(-1);
	}
	
	printf("program_name: %s\n", fname);
	printf("cache_size: ");
	scanf("%d", &size);
	printf("block_size: ");
	scanf("%d", &block_size);

	index = (int)(log((size / block_size)) / log(2));

	for (i = 0; i < index; i++)
		for (j = 0; j < 256; j++)
			cache[i][j].vaild = 0;

	for (i = 0; i < 1500000 && (fscanf(f, "%ld", &diff) != EOF); i++) {
			ea = ea + diff;

			cache_index = (ea % size) / block_size;
			tag = ea / size;

			if (cache[cache_index][0].vaild == 0) {
				miss++;
				cache[cache_index][0].tag = tag;
				cache[cache_index][0].vaild = 1;
			}
			else {
				if (cache[cache_index][0].tag == tag) {
					hit++;
				}
				else {
					miss++;
					cache[cache_index][0].tag = tag;
				}
			}
		
		
	}

	printf("total_lds: 1500000\n");
	printf("Hit: %d\nMiss: %d\nMiss Rate: %f", hit, miss, miss / 1500000.0);
}