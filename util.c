#include "util.h"

void swap(double *a, double* b){
    double t = *a;
    *a = *b;
    *b = t;
}

void quickSort( double arr[], int l, int r)
{
   int j;
   if( l < r ) {
		j = partition( arr, l, r);
		quickSort( arr, l, j-1);
		quickSort( arr, j+1, r);
   }
}



int partition( double arr[], int l, int r) {
	double pivot, t;
	int i, j;
	pivot = arr[l];
	i = l; j = r+1;
		
	while( 1){
		do ++i; while( arr[i] <= pivot && i <= r );
		do --j; while( arr[j] > pivot );
		if( i >= j ) break;
			swap(&arr[i], &arr[j]);
	}
    swap(&arr[l], &arr[j]);
	return j;
}
