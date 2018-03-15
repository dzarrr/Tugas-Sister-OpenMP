/*
 bitonic.c 

 This file contains two different implementations of the bitonic sort
        recursive  version 
        imperative version :  impBitonicSort() 
 

 The bitonic sort is also known as Batcher Sort. 
 For a reference of the algorithm, see the article titled 
 Sorting networks and their applications by K. E. Batcher in 1968 


 The following codes take references to the codes avaiable at 

 http://www.cag.lcs.mit.edu/streamit/results/bitonic/code/c/bitonic.c

 http://www.tools-of-computing.com/tc/CS/Sorts/bitonic_sort.htm

 http://www.iti.fh-flensburg.de/lang/algorithmen/sortieren/bitonic/bitonicen.htm 
 */

/* 
------- ---------------------- 
   Nikos Pitsianis, Duke CS 
-----------------------------
*/


#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

struct timeval startwtime, endwtime;
double seq_time;


int N;          // data array size
int *a;         // data array to be sorted

const int ASCENDING  = 1;
const int DESCENDING = 0;


void init(void);
void print(void);
void sort(void);
void test(void);
inline void exchange(int i, int j);
void compare(int i, int j, int dir);
void bitonicMerge(int lo, int cnt, int dir);
void recBitonicSort(int lo, int cnt, int dir);
void impBitonicSort(int paddedN);
void impBitonicSortParallel(int numberOfThread, int paddedN);
void rng(int* arr, int n);
int getPaddedN(int N);
int findMax(int* arr, int N);
/** the main program **/ 
int main(int argc, char **argv) {

  if (argc != 3) {
    printf("Usage: %s n\n  where n is problem size (power of two)\n", argv[0]);
    exit(1);
  }

  N = atoi(argv[1]);

  int paddedN = getPaddedN(N);

  int numberOfThread = atoi(argv[2]);
  a = (int *) malloc(paddedN * sizeof(int));

  rng(a, N);
  printf("Padded N : %d\n", paddedN);
  int maxNum = findMax(a, N);
  for (int i = N; i < paddedN; i++){
    a[i] = maxNum;
    printf ("%d\n", a[i]);
  }
  printf("INISTARTYA\n");

  gettimeofday (&startwtime, NULL);
  impBitonicSortParallel(numberOfThread, paddedN);
  gettimeofday (&endwtime, NULL);

  for (int i = 0; i < paddedN; i++){
    if (i == N){
      printf("++++++++++++++++++++++++\n");
    }
    printf("%d\n", a[i]);
  }

  seq_time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6
		      + endwtime.tv_sec - startwtime.tv_sec);

  printf("Parallel imperative wall clock time = %f\n", seq_time);
  test();

  rng(a, N);
  gettimeofday (&startwtime, NULL);
  impBitonicSort(paddedN);
  gettimeofday (&endwtime, NULL);

  seq_time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6
		      + endwtime.tv_sec - startwtime.tv_sec);

  printf("Serial imperative wall clock time = %f\n", seq_time);
  test();

  //print();
}

/** -------------- SUB-PROCEDURES  ----------------- **/ 

/** procedure test() : verify sort results **/
void test() {
  int pass = 1;
  int i;
  for (i = 1; i < N; i++) {
    pass &= (a[i-1] <= a[i]);
  }

  printf(" TEST %s\n",(pass) ? "PASSed" : "FAILed");
}


/** procedure init() : initialize array "a" with data **/
void init() {
  int i;
  for (i = 0; i < N; i++) {
     a[i] = rand() % N; // (N - i);
    a[i] = (N - i);
  }
}

void rng(int* arr, int n) {
    int seed = 13515007; // Ganti dengan NIM anda sebagai seed.
    srand(seed);
    for(long i = 0; i < n; i++) {
        arr[i] = (int)rand();
    }
}

/** procedure  print() : print array elements **/
void print() {
  int i;
  for (i = 0; i < N; i++) {
    printf("%d\n", a[i]);
  }
  printf("\n");
}


/** INLINE procedure exchange() : pair swap **/
inline void exchange(int i, int j) {
  int t;
  t = a[i];
  a[i] = a[j];
  a[j] = t;
}

int getPaddedN(int N){
  int duaPangkat = 1;
  while (duaPangkat < N) {
    duaPangkat *= 2;
  }
  return duaPangkat;
}

/** procedure compare() 
   The parameter dir indicates the sorting direction, ASCENDING 
   or DESCENDING; if (a[i] > a[j]) agrees with the direction, 
   then a[i] and a[j] are interchanged.
**/
void compare(int i, int j, int dir) {
  if (dir==(a[i]>a[j])) 
    exchange(i,j);
}




/** Procedure bitonicMerge() 
   It recursively sorts a bitonic sequence in ascending order, 
   if dir = ASCENDING, and in descending order otherwise. 
   The sequence to be sorted starts at index position lo,
   the parameter cbt is the number of elements to be sorted. 
 **/
void bitonicMerge(int lo, int cnt, int dir) {
  if (cnt>1) {
    int k=cnt/2;
    int i;
    for (i=lo; i<lo+k; i++)
      compare(i, i+k, dir);
    bitonicMerge(lo, k, dir);
    bitonicMerge(lo+k, k, dir);
  }
}



/** function recBitonicSort() 
    first produces a bitonic sequence by recursively sorting 
    its two halves in opposite sorting orders, and then
    calls bitonicMerge to make them in the same order 
 **/
void recBitonicSort(int lo, int cnt, int dir) {
  if (cnt>1) {
    int k=cnt/2;
    recBitonicSort(lo, k, ASCENDING);
    recBitonicSort(lo+k, k, DESCENDING);
    bitonicMerge(lo, cnt, dir);
  }
}


/** function sort() 
   Caller of recBitonicSort for sorting the entire array of length N 
   in ASCENDING order
 **/
void sort() {
  recBitonicSort(0, N, ASCENDING);
}



/*
  imperative version of bitonic sort
*/
void impBitonicSortParallel(int numberOfThread, int paddedN) {

  int i,j,k;
  
  for (k=2; k<=paddedN; k=2*k) {
    for (j=k>>1; j>0; j=j>>1) {
      #pragma omp parallel for num_threads(numberOfThread)
      for (i=0; i<paddedN; i++) {
	int ij=i^j;
	if ((ij)>i) {
	  if ((i&k)==0 && a[i] > a[ij]) 
	      exchange(i,ij);
	  if ((i&k)!=0 && a[i] < a[ij])
	      exchange(i,ij);
	}
      }
    }
  }
}


void impBitonicSort(int paddedN) {

  int i,j,k;
  
  for (k=2; k<=paddedN; k=2*k) {
    for (j=k>>1; j>0; j=j>>1) {
      for (i=0; i<paddedN; i++) {
  int ij=i^j;
  if ((ij)>i) {
    if ((i&k)==0 && a[i] > a[ij]) 
        exchange(i,ij);
    if ((i&k)!=0 && a[i] < a[ij])
        exchange(i,ij);
  }
      }
    }
  }
}


int findMax(int* arr, int N){
  int max = arr[0];
  for (int i = 0; i < N; i++){
    if (arr[i] > max){
      max = arr[i]; 
    }
  }
  return max;
}