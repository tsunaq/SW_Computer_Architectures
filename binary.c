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