#include "readCSVFile.h"

char line[MAX_LINE_LENGTH];

CSV_Data* allocCSVHeader()
{
	CSV_Data* head = (CSV_Data*)malloc(sizeof(CSV_Data));
	memset(head, 0, sizeof(CSV_Data));
	return head;
}

/*
allocate a CSV_Data structure which can store rowNum*colNum int
and initiate the space.
Return NULL if allocation fails.
*/
CSV_Data* allocCSVData(int rowNum, int colNum){
	CSV_Data* newData = (CSV_Data*)malloc(sizeof(CSV_Data));
	if (newData == NULL) return NULL;

	newData->data = (float*)malloc(sizeof(float)*rowNum*colNum);
	if (newData->data == NULL){
		free(newData);
		return NULL;
	}

	newData->rows = 0; // empty data space, so no valid rows
	newData->rowNum = rowNum;
	newData->colNum = colNum;
	newData->next = NULL;
	memset(newData->data, 0, sizeof(float)*rowNum*colNum);
	return newData;
}

/*
free a CSV_Data structure and the data array it points to
*/
void freeCSVData(CSV_Data* p){
	free(p->data); // free the data array
	free(p); // free CSV_Data structure
}

/*
free a list of CSV_Data, initialize the header to 0.
should the head of CSVDataList (not the dummy head), i.e., dummyHead.next
*/
void freeCSVDataList(CSV_Data* head){
	CSV_Data* tmp, * p = head->next;
	// free the list
	while (p != NULL) {
		tmp = p->next;
		freeCSVData(p);
		p = tmp;
	}
	memset(head, 0, sizeof(CSV_Data));
}

/*
Parsing a line of the CSV file and count the # of columns.
return -1 if error for parsing.
otherwise return the # of columns.
*/
int getCSVColNum(FILE * fp){
	//char line[MAX_LINE_LENGTH] = {0x0};
	char * mline = line;
	line[0] = '\0';
	int i = 0;
	char* ptr;
	//char *line, *mline;
	//size_t len;
	
	//if (getline(&line, &len, fp))
	if (fgets(line, MAX_LINE_LENGTH, fp))
	{	
		mline = line;
		for(int i = 0; i < 100; i++)
			printf("%c", line[i]);
		printf(" strlen %d\n", strlen(line));
		fflush(stdout);
		ptr = strtok(mline, ",");
		while(ptr != NULL){
			i ++;
			printf("%s %s %s\n", ptr, mline, line);
			ptr = strtok(NULL, ",");
		}
	}
	//free(line);
	fseek(fp, 0, SEEK_SET);
	return i;
}


/* 
Read the CSVData and store it into the list given in head,
allocate new trunck of CSV data if needed, each truck has DEFAULT_CSV_DATA_ROW rows.
return 0 if success, -1 if fail.
*/
int readCSVData(CSV_Data* head, char* filePath)
{
	FILE* fp;
	char* ptr;
	char csvline[MAX_LINE_LENGTH];
	int rowNum = DEFAULT_CSV_DATA_ROW;

	CSV_Data* p = head;

	int colNum; 
	int i = 0, j = 0; // i count the row and j count the 
 	int error = 0;
	
	if( (fp = fopen(filePath, "r" )) == NULL )
	{
		//printf("Can't open file %s",filePath);
		error = FILE_OPEN_FAIL;
		goto fail;
	}

	colNum = getCSVColNum(fp); // get the # of columns in first line

	if (colNum <= 0){
		error = FILE_PARSE_FAIL;
		goto fail;
	}
	printf("get column %d\n", colNum);

	head->colNum = colNum;
	
	while (fgets(csvline, MAX_LINE_LENGTH, fp)){
		//printf("read file %s\n", csvline);
		// if the current CSV_Data structure if full
		// allocate new CSV_Data structure
		if (p->rows == p->rowNum ){ 
			p->next = allocCSVData(rowNum, colNum); // allocate next CSV_Data
			if (p->next == NULL) { //handling allocation fail
				error = MEM_ALLOC_FAIL;
				goto fail;
			}
			head->rows += p->rows; // update valid rows,
			head->rowNum += rowNum; // update row capacity.
			p = p->next;
		}
		j = 0;
		// parsing each value in a row
		ptr = strtok(csvline, ","); 
		while(ptr != NULL)
		{
			p->data[p->rows*colNum + j++] = atof(ptr);
			if(errno == ERANGE)
				error = CSV_DATA_TOO_LARGE;
			else if (errno == EINVAL)
				error = CSV_DATA_INVALID;
			if (error != 0){
				goto fail;
			}
			ptr = strtok(NULL, ",");
		}
		p->rows ++;
	}
	head->rows += p->rows;

	fclose(fp);
	return 0;

fail: //clear up allocation and close file
	freeCSVDataList(head); 
	fclose(fp); 
	return error;
}

/*
print the column col from the CSV_Data List
return 0 if success, otherwise error code
*/
int getColumn(CSV_Data* head, int col, float* colData){
	// colNum should be from 0 to colNum -1
	CSV_Data* p = head->next;
	colData = (float*)malloc(sizeof(float)*head->rows);
	if (colData == NULL) {
		free(colData);
		return MEM_ALLOC_FAIL;
	}

	int i = 0; // indices 
	while (p != NULL){
		for (int k = 0; k < p->rows; i++)
			colData[i++] = (p->data[p->colNum*k + col]); 
		p = p->next;
	}
	return 0;
}
// void getRow(CSV_Data* head, int row, int* array){}

/*
get two column and allocate space, and put them in colData1, and colData2
*/
int getTwoColumns(CSV_Data* head, int col1, int col2, float* colData1, float* colData2){
	int error = 0;
	error = getColumn(head, col1, colData1);
	if (error != 0)
		goto fail;
	error = getColumn(head, col2, colData2);
	if (error != 0)
		goto fail;
	return 0;

fail:
	free(colData1);
	free(colData2);
	return error;
}


/* output a column value to the file descripter*/
void outputColumn( float* colData, int len, FILE* output){
	for (int i = 0; i < len; i++)
		fprintf(output, "%f\t", colData[i]);
	fprintf(output, "\n");
}

// int addColumn(CSV_Data* head, int col1, int col2, int result[]){
// 	if (col1 >= head->rows){
// 		//printf("Column number %d is out of range.\n", col1);
// 		return OUT_OF_RANGE;
// 	}
// 	if (col2 >= head->rows){
// 		printf("Column number %d is out of range.\n", col2);
// 		return OUT_OF_RANGE;
// 	}
// 	float* colData = addColumn(head, col1, col2);
// 	if (colData == NULL) return ;

// 	printf("The sum for column %d and column %d is: \n", col1, col2);
// 	outputColumn(colData, head->rows)
// 	free(colData);
// }

/* 
Parse the error and print error information
*/
void parseErrInfo(FILE* output, int error){
	switch (error){
		case FILE_OPEN_FAIL:
			fprintf(output, "Fail to open the file.\n");
			break;
		case FILE_PARSE_FAIL:
			fprintf(output, "Fail to parse the file.\n");
			break;
		case MEM_ALLOC_FAIL:
			fprintf(output, "Memory allocation fail.\n");
			break;
		case COL_OUT_OF_RANGE:
			fprintf(output, "Given column is out of range.\n");
			break;
		case CSV_DATA_TOO_LARGE:
			fprintf(output, "CSV file data is too large, only float size is supported.\n");
			break;
		case CSV_DATA_INVALID:
			fprintf(output, "CSV file data is invalid, only float data is supported.\n");
			break;
	}

}

