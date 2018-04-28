#include "cmdopt.h"


command cmdList[COMMAND_SIZE] = {
	{LOAD_COMMAND, "load", 2, "  load  file - load the csv file into memory.", loadCommand},
	{COL_COMMAND, "col", 2, "  col num1 - show the column data for column num1.", colCommand},
	{STAT_COMMAND, "stat", 2, "  stat num1 - show the statistics for column num1.", statCommand},
	{ADDCOL_COMMAND, "addcol", 3, "  addcol num1 num2 - add two columns.", addColCommand},
	{SUBCOL_COMMAND, "subcol", 3, "  subcol num1 num2 - substract two columns.", subColCommand},
	{MULCOL_COMMAND, "mulcol", 3, "  mulcol num1 num2 - multiple two columns.", mulColCommand},
	{DIVCOL_COMMAND, "divcol", 3, "  divcol num1 num2 - divide two columns (0 if divided by 0).", divColCommand}
};


/*
print the usage of all command 
*/
void printUsage(){
	int i;
	printf("Usage:");
	for(i = 0; i < COMMAND_SIZE; i++)
		printf("%s\n", cmdList[i].cmdHelp);
	printf("\n");
}

// print the usage of a perticular command.
void printTypeUsage(int type){
	printf("Usage:");
	if (type >= 0 && type < COMMAND_SIZE) // within the range
		printf("%s\n\n", cmdList[type].cmdHelp);
}

/*
Parsing a string as should be valid string as one of the command type.
Return the enum value of the command type.
*/
int parseCommand(char* line, int* argc, char* argv[]){
	char* ptr = strtok(line, " ");
	int count = 0, i;
	while(ptr != NULL){
		argv[count++] = ptr;
		ptr = strtok(NULL, " ");
	}
	*argc = count;

	for (i = 0; i < COMMAND_SIZE; i ++){
		if (strcmp(argv[0], cmdList[i].cmdName) == 0){
			return cmdList[i].id;
		}
	}
	return -1;
}

void runCommand(CSV_Data* head, int argc, char* argv[], int type){
	cmdList[type].handler(head, argc, argv);
}

int checkValid(CSV_Data* head, int argc, char* argv[], int type){
	int i, col;
	if (type < 0 || type >= COMMAND_SIZE) {
			printf("Command not supported!\n\n");
			printUsage();
			return -1;
	}
	if (head->next == NULL && type != LOAD_COMMAND){
		printf("CSV data is not loaded, use load command to load CSV file first.\n");
		return -1;
	}
	if (cmdList[type].paras != argc){
		printf("Wrong number of parameters for command %s \n", cmdList[type].cmdName);
		printTypeUsage(type);
		return -1;
	}

	// if not load, then all parameters are column number
	// check whether its a valid column number
	if (type != LOAD_COMMAND){ 
		for(i = 1; i < cmdList[type].paras; i++){
			col = atoi(argv[i]) -1;
			if (errno == EINVAL || errno == ERANGE){
				printf("Column number should be an integer.\n");
				return -1;
			}
			if (col >= head->colNum || col < 0){
				printf("Column number %d is out of range.\n", col);
				printf("Valid column number should be from 1 to %d\n", head->colNum);
				return -1;
			}
		}
	}
	return 0;
}

void loadCommand(CSV_Data* head, int argc, char* argv[]){
	freeCSVDataList(head);
	int error = readCSVData(head, argv[1]);
	if (error == 0){
		printf("Load file %s into memory.\n", argv[1]);
		printf("File Info: %d rows and %d columns.\n", head->rows, head->colNum);
	}
	else{
		parseErrInfo(stdout, error);
	}
}


void colCommand(CSV_Data* head, int argc, char* argv[]){
	int col = atoi(argv[1]) -1;
	double* colData;
	int error = getColumn(head, col, &colData);

	// print error infor if getColumn return non zero.
	if (error != 0){
		parseErrInfo(stdout, error);
		free(colData);
		return ;
	}

	// print out the content.
	printf("The content for column %d is: \n", col);
	outputColumn(colData, head->rows, stdout);
	free(colData);
}

void statCommand(CSV_Data* head, int argc, char* argv[]){
	int col = atoi(argv[1]) -1;

	double* colData;
	// memset(colData, 0, sizeof(int)*head->rows);
	double max, min, median;
	int mid = head->rows/2;
	int error;
	// get column error, then return
	error = getColumn(head, col, &colData);
	if (error != 0){
		parseErrInfo(stdout, error);
		return ;
	}
	// sort in place
	quickSort(colData, 0, head->rows-1);
	min = colData[0];
	max = colData[head->rows-1];

	// get median
	median = head->rows % 2 == 0 ? (colData[mid-1] + colData[mid])/2 : colData[mid]; 

	outputColumn(colData, head->rows, stdout);
	quickSort(colData, 0, head->rows-1);

	printf("first data is %f\n", head->next->data[0]);

	printf("The statistic of column %d: \n", col);
	printf("max: %f, min: %f, median: %f\n", max, min, median);

	outputColumn(colData, head->rows, stdout);
	free(colData);
}

/* 
add two giving columns and output the sum in an array
*/
void addColCommand(CSV_Data* head, int argc, char* argv[]){
	int col1, col2, i;
	col1 = atoi(argv[1]) -1;
	col2 = atoi(argv[2]) -1;

	double* colData1, * colData2;
	int error = getTwoColumns(head, col1, col2, &colData1, &colData2);
	if (error != 0){
		parseErrInfo(stdout, error);
		return ;
	}
	for(i = 0; i < head->rows; i++){
		colData1[i] += colData2[i];
	}

	outputColumn(colData1, head->rows, stdout);
	free(colData2);
	free(colData1);
}


/* 
subtract two giving columns and return the result in an array
*/
void subColCommand(CSV_Data* head, int argc, char* argv[]){
	int col1, col2, i;
	col1 = atoi(argv[1]) -1;
	col2 = atoi(argv[2]) -1;

	double* colData1, * colData2;
	int error = getTwoColumns(head, col1, col2, &colData1, &colData2);
	if (error != 0){
		parseErrInfo(stdout, error);
		return ;
	}
	for(i = 0; i < head->rows; i++){
		colData1[i] -= colData2[i];
	}

	outputColumn(colData1, head->rows, stdout);
	free(colData2);
	free(colData1);
}

/* 
multiple two giving columns and return the result in an array
*/
void mulColCommand(CSV_Data* head, int argc, char* argv[]){
	int col1, col2, i;
	col1 = atoi(argv[1]) -1;
	col2 = atoi(argv[2]) -1;

	double* colData1, * colData2;
	int error = getTwoColumns(head, col1, col2, &colData1, &colData2);
	if (error != 0){
		parseErrInfo(stdout, error);
		return ;
	}
	for(i = 0; i < head->rows; i++){
		colData1[i] *= colData2[i];
	}

	outputColumn(colData1, head->rows, stdout);
	free(colData2);
	free(colData1);
}

/* 
divide two giving columns and return the result in an array
if divide by 0, set to 0.
*/
void divColCommand(CSV_Data* head, int argc, char* argv[]){
	int col1, col2, i;
	col1 = atoi(argv[1]) -1;
	col2 = atoi(argv[2]) -1;

	double* colData1, * colData2;
	int error = getTwoColumns(head, col1, col2, &colData1, &colData2);
	if (error != 0){
		parseErrInfo(stdout, error);
		return ;
	}
	for(i = 0; i < head->rows; i++){
		if (colData2[i] != 0)
			colData1[i] /= colData2[i];
		else
			colData1[i] = 0;
	}

	outputColumn(colData1, head->rows, stdout);
	free(colData2);
	free(colData1);
}


