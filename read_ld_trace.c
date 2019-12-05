
/*The following program takes the name of the compressed trace file as
input and prints out the first 10 load  effective addresses. The trace
in hex file contains 1 entry per line. This entry is the difference
between the effective address of this load and the effective address of
the previous one. The effective address is assumed to be 0 at the
beginning(i.e. the first line is the actual effective address of the first
load in the trace).*/

#include<stdio.h>

void main(int argc,char *argv[]){

  FILE * f;
  char fname[50];
  long diff;
  unsigned long ea = 0;
  int i;

  if(argc > 1){
    /*If there is a command line argument, use it as the gzip
     file name*/
    sprintf(fname,"%s",argv[1]);
    printf("%s\n",fname);
  }
  else{
    /*If there are no command line arguments, ask for one and exit*/
    printf("Need to enter trace file name as parameter.\n");
    exit(-1);
  }

  f = fopen(fname,"r");
  if (!f) {
    /*If the given file does not exist, tell the user and exit*/
    printf("Need to create %s first.\n",argv[1]);
    exit(-1);
  }


  for(i=0; i < 10 && (fscanf(f,"%ld",&diff) != EOF);i++){
    ea = ea + diff;
    printf("%ld \n",ea);
  }
}

