#ifndef READCSVFILE_H

# define READCSVFILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "util.h"

#define MAX_LINE_LENGTH 1024 // the maximum length allows in a csv file 
#define MAX_FILEPATH_LEN 256 // the maximum length of filepath
#define DEFAULT_CSV_DATA_ROW 100
#define DEFAULT_CSV_DATA_SIZE 1024 // the default memory size in bytes allocated for the data field in CSV_DATA



// error code for the function return.
#define FILE_OPEN_FAIL 			1
#define FILE_PARSE_FAIL 		2
#define MEM_ALLOC_FAIL 			3
#define COL_OUT_OF_RANGE 		4
#define CSV_DATA_TOO_LARGE		5
#define CSV_DATA_INVALID		6


/* 
the basic structure to store the csv data in memory
Noted that for large CSV file, we avoid preallocate large truck of memeory
instead we use a pointer to point to the next truck of CSV data.
We use this structure to create a linked list.
The header of the list also uses the same CSV_Data structure, except the data file is not used.
*/
struct Data{
	int rows;	// # of valid rows in data 
	int rowNum; // # of rows can store in data 
	int colNum; // # of cols can store in data
	float* data; 
	struct Data* next;
};
typedef struct Data CSV_Data;

// functions to allocate and free CSV_Data structures
CSV_Data* allocCSVHeader();
CSV_Data* allocCSVData(int rowNum, int colNum);
void freeCSVData(CSV_Data* p);
void freeCSVDataList(CSV_Data* head);


// functions for loading CSV and getting column data.
int getCSVColNum(FILE * fp);
int readCSVData(CSV_Data* head, char* filePath);
int getColumn(CSV_Data* head, int col, float* colData);
int getTwoColumns(CSV_Data* head, int col1, int col2, float* colData1, float* colData2);

void outputColumn( float* colData, int len, FILE* output);

void parseErrInfo(FILE* output, int error);




#endif