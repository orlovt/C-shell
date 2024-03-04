#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdbool.h>

#include "util.h"

#define MAX_INPUT_LENGTH 1024

int main() {
    char input[MAX_INPUT_LENGTH];
    char *args[MAX_INPUT_LENGTH / 2];  // var for the arguments
    int should_run = 1;  // flag to determine when to exit program

	char cwd[1024];
	getcwd(cwd, sizeof(cwd)); // cwd for printing the path
	// allocating paths since we want to use our executables 
	// ./ls, ./cs, ./wc
	char ls_path[1024];
	char cd_path[1024];
	char wc_path[1024];
	// snprintf to create the path
	snprintf(ls_path, sizeof(ls_path), "%s/ls", cwd);
	snprintf(cd_path, sizeof(cd_path), "%s/cd", cwd);
	snprintf(wc_path, sizeof(wc_path), "%s/wc", cwd);

	// main loop
    while (should_run) {
		// promt the user
        printf("[shell]%s$ ", getcwd(NULL, 0)); 
        fflush(stdout);

        // read input
        if (!fgets(input, MAX_INPUT_LENGTH, stdin)) {
            fprintf(stderr, "Failed to read input\n");
            continue;
        }

		// removing "\n"
		input[strlen(input) - 1] = '\0'; 
		// trimming the input
		char *trimmed_input = trimwhitespace(input); 

		// parsing input into tokens
		char *args[MAX_INPUT_LENGTH / 2];
		// split on ' '
		int num_args = parse_line(trimmed_input, args, " "); 
		// check if the input is non empty
		if (num_args > 0) { // if there are commands to execute
			execute_cmd(args, ls_path, wc_path, &should_run); // execute
		}

    }
	// exit shell
	printf("Exiting shell\n");
    return 0; 
}
