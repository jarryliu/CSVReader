# Program Manual

## COMPILE AND RUN

Use make command to compile the program.
Run the program, it prompts to get input from user.

```bash
$ make    # compile the program
$ ./csvcmd
Type the command (press enter to exit):	
```

## SUPPORTED COMMANDS

There are 7 commands supported:

- `load` command, e.g., `load test.csv`, with a file path as parameter to load the given file into memory.

- `col` command, e.g., `col 2`, with a column number as parameter to print out the data in the column.

- `stat` command, e.g., `stat 3`, with a column number as parameter to print out the statistics information (max, min, and median) of the given column.

- `addcol` command, e.g., `addcol 1 2`, with two column numbers to add the two columns and print out the results, i.e., col 1 + col 2.

- `subcol` command, e.g., `subcol 1 2`, with two column numbers to substract the latter column from former column and output the results, i.e., col 1 - col 2.

- `mulcol` command, e.g., `mulcol 1 2`, with two column numbers to multiple two columns and print out the results, i.e., col 1 * col 2.

- `divcol` command, e.g., `divcol 1 2`, with two column numbers to divide the former column by the latter one and print out the results, i.e., col 1 / col 2.


`load` command need to be run before any other commands. CSV file of number is supported, all data form will not be correctly parsed. 

Only one file can be loaded into memory at a time, previous data will be freed. 

## OTHERS

To get help information, type anything and press enter.

To quit the program, directly press enter.

The maximum line length for CSV file is 1024*16 bytes. The default number of rows space to allocate is 100.

To increase the capability of the program handling large file, you can change the  macros MAX_LINE_LENGTH and DEFAULT_CSV_DATA_ROW. 

