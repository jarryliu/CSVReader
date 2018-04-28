#include "readCSVFile.h"
#include "cmdopt.h"

#define MAX_INPUT_LEN 200

extern command cmdList[COMMAND_SIZE];

int main(int argc, char *argv[])
{
	CSV_Data head = {0, 0, 0, NULL, NULL}; 	// dummy head is allocated sperately
	char* line = NULL;		// buff to hold user input
	size_t len = 0;
	//char filePath[MAX_FILEPATH_LEN] = {'\0'};	// buff to hold filepath, initialized as empty string
	
	int cmdType; 
	int parac;
	char* parav[MAX_CMD_PARA_NUM]; 
	
	printf("\nType the command (press enter to exit):\t");
	while(getline(&line, &len, stdin) != -1){
		// parsing the command
		line[strlen(line) - 1] = '\0'; // remove '\n' in the end of the string.
		if (strlen(line) == 0) // press enter to exit
			break;
		cmdType = parseCommand(line, &parac, parav);
		
		// check the parameter number, type 
		if (checkValid(&head, parac, parav, cmdType) == 0){
			// call handler function
			runCommand(&head, parac, parav, cmdType);
		}
		printf("\nType the command (press enter to exit):\t");
	}

	printf("\nQuit program.\n");
	free(line);
	freeCSVDataList(&head);
	return 0;
}
