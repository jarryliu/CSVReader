#ifndef CMD_OPTION_H

#define CMD_OPTION_H

#include "readCSVFile.h"


#define COMMAND_SIZE 7
#define MAX_CMD_PARA_NUM 10

// error code
#define WRONG_COMMAND 	1
#define WRONG_PARA_NUM 	2

// command id
#define LOAD_COMMAND 	0
#define COL_COMMAND		1
#define STAT_COMMAND	2
#define ADDCOL_COMMAND	3
#define SUBCOL_COMMAND	4
#define MULCOL_COMMAND	5
#define DIVCOL_COMMAND	6

//typedef enum {LOAD_COMMAND, COL_COMMAND, STAT_COMMAND, ADDCOL_COMMAND, SUBCOL_COMMAND, MULCOL_COMMAND, DIVCOL_COMMAND} cmdType;

typedef struct  {
	int id; 	// the commond id for the command, as 
	char* cmdName;	// the command name
	int paras; 		// # of parameters for the command
	char* cmdHelp;	// help information for the command 
	void (*handler)(CSV_Data* head, int argc, char* argv[]); // call function for the command.
} command;

void printUsage();
void printTypeUsage(int type);

int checkValid(CSV_Data* head, int argc, char* argv[], int type);
int parseCommand(char* line, int* argc, char* argv[]);
void runCommand(CSV_Data* head, int argc, char* argv[], int type);

// the handler functions for each command
void loadCommand(CSV_Data* head, int argc, char* argv[]);
void colCommand(CSV_Data* head, int argc, char* argv[]);
void statCommand(CSV_Data* head, int argc, char* argv[]);
void addColCommand(CSV_Data* head, int argc, char* argv[]);
void subColCommand(CSV_Data* head, int argc, char* argv[]);
void mulColCommand(CSV_Data* head, int argc, char* argv[]);
void divColCommand(CSV_Data* head, int argc, char* argv[]);


#endif