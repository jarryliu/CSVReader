#include "util.h"


// swap two number
void swap(float* a, float* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

int partition (float arr[], int low, int high)
{
    int pivot = arr[high]; 
    int i = low-1, j; 
 
    for (j = low; j <= high- 1; j++)
    {
        if (arr[j] <= pivot)
        {
            i++; 
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}
 
// use quickSort to sort the array in-place
void quickSort(float arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);

        // recursively call
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}