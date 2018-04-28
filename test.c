#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_SIZE 50

int main(int argc, const char * argv[]) {
    
    char file_name[] = "test.txt";
    FILE *fp;
    fp = fopen(file_name, "r");
    
    if (!fp) {
        fprintf(stderr, "failed to open file for reading\n");
        return 1;
    }
    
    char line[MAX_LINE_SIZE];
    char *result = NULL;
    
    while(fgets(line, MAX_LINE_SIZE, fp) != NULL) {
        printf("%s\n", line);
        result = strtok(line, ",");

        while( result != NULL ) {
            printf("%s\t", result);
            result = strtok(NULL, ",");
        }
        printf("\n");
    }
    
    fclose (fp);
    
    return 0;
}
