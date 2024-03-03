#define MAX_CMD_LEN 256
#define MAX_TOKENS 100

enum command_type {LS=0, CD=1, WC=2, EXIT=3, ERROR=4};


void execute_cmd(char *args[], char *ls_path, char *wc_path, int *should_run);

/* Parses the input string passed to it
 * Takes in a pointer to the array of tokens
 * returns the total number of tokens read
 */
int parse_line(char * input, char * tokens[], char * delim);

/* Returns the command type of the command passed to it */
enum command_type get_command_type(char *command);

/* Removes the leading and trailing white spaces in the give string.
 * This function modifies the given string.
 */
char *trimwhitespace(char *str);

/* The function handles redirection is we use | in the comand 
* It takes in the array of tokens and handles the redirection inside of the process
*/ 
void handle_redirection(char *args[]); 

/* Handles the execution of commands with pipes.
 * This function takes an array of arguments `args` and executes the commands
 * with pipes. It sets up the necessary pipes, forks child processes, and
 * redirects the input and output accordingly. The function also waits for the
 * child processes to complete before returning.
 */
void handle_pipes(char *args[]); 


/* This function takes an array of command arguments and checks if any of the arguments
 * contain a pipe symbol ('|'). It returns true if a pipe symbol is found, and false otherwise.
 */
bool check_for_pipe(char *args[]); 


