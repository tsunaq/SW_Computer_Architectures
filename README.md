# SW_Computer_Architectures

## 1. 설계목적
Data cache simulator를 구현해 봄으로써 N-way associative cache의 동작원리를 이해하도록 함.

## 2. 설계방법
본 설계과제에서 cache 사이즈를 변화(2의 승수에서 64KB까지) 시킬 수 있는 N-way associative cache  
를 설계한다. Cache set에서 replace되는 way/block을 선택하기 위해 LRU (Least Recently Used) 방식  
을 선택하라. 시뮬레이션을 위해 BYTE addressable 메모리 계층구조를 가정하라. 여러분이 구현할 시뮬  
레이터는 다음과 같은 4가지 변수가 실행되도록 하여야 한다.  

```
-t input_trace_file
-s size
-a associativity
-b block_size
```

Cache size (-s)와 block size (-b)는 byte로 수행하고, associativity는 2의 승수로 수행되어야 한다.  
Input trace file은 첨부한 go_ld_trace (ZIP file)를 사용하라. 이 파일은 어떤 프로그램의 실행된  
load 명령어들의 trace를 포함하고 있다. 각 라인은 실행된 load 명령어에 의해 접근된 바이트 주소로  
표현되어 있다. Trace를 읽는 예는 read_ld_trace.c에서 기술되어 있다. 여러분은 이 파일을 사용하고,  
아래 보이는 trace의 시작부분부터 사용하여 캐쉬 시뮬레이터를 작성하라.  

Here is the start of the trace file "go_ld_trace trace" you will be working with; once restored  
(make sure you see the same load addresses) is:  
536869800  
536869816  
1073839984  
1073867968  
1074199644  
1074170256  
536869696  
1073839832  
1074229644  
1074196644  

The output of running your cache simulator should look like:  
>cache-sim -t go_ld_trace -s 1024 -b 32 -a 2  
program_name: go_ld_trace  
cache_size:1024  
block_size:32  
associativity:2  
total_lds:1500000  
cache_hits:1005939  
cache_misses:494061  
cache_miss_rate:0.33  
The maximum values your simulator needs to support for each of the parameters (s,b,a) is 64K  
bytes for the cache size, 256 bytes for the block size, and the full associativity.  

## 3. 결과
![1](https://user-images.githubusercontent.com/58457978/70210572-5baf0d00-1776-11ea-8e28-b82a1c8a881c.png)

![2](https://user-images.githubusercontent.com/58457978/70210573-5baf0d00-1776-11ea-97fc-8716d62182ee.png)

![3](https://user-images.githubusercontent.com/58457978/70210570-5b167680-1776-11ea-974a-22d5213bc576.png)

![4](https://user-images.githubusercontent.com/58457978/70210571-5baf0d00-1776-11ea-836e-e229eca1c957.png)

## 4. 코드(binary.c)
<pre><code>
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
#include <math.h>

char str[33];
char str2[33];

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

	FILE *f;
	FILE *fp;
	char fname[50];
	long diff;
	unsigned long ea = 0;
	int i;

	if ((fp = fopen("memory.txt", "w+")) == NULL) {
		fprintf(stderr, "입력을 위한 파일을 열 수 없습니다");
		exit(1);
	}

	if (argc > 1) {
		/*If there is a command line argument, use it as the gzip
		file name*/
		sprintf(fname, "%s", argv[1]);
	}
	else {
		/*If there are no command line arguments, ask for one and exit*/
		printf("Need to enter trace file name as parameter.\n");
		exit(-1);
	}

	f = fopen(fname, "r");
	if (!f) {
		/*If the given file does not exist, tell the user and exit*/
		printf("Need to create %s first.\n", argv[1]);
		exit(-1);
	}

	for (i = 0; i < 1500000 && (fscanf(f, "%ld", &diff) != EOF); i++) {
		ea = ea + diff;
		bin(ea); // str에 32비트 문자열로 저장
		
		fputs(itoa(i+1, str2, 10), fp);
		fputs(" ", fp);
		fputs(str, fp);
		fputs("\n", fp);
	}
	printf("Success!");
}
<\code><\pre>

## 5. 코드(read_ld_trace.c)
<pre><code>
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
<\code><\pre>
