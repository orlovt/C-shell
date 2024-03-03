  #include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "util.h"
#include <limits.h>
#include <ctype.h>
#include <fcntl.h>      // For open() function and O_* constants
#include <sys/types.h>  // For mode_t type
#include <sys/wait.h>   // For waitpid()
#include <sys/stat.h>   // For S_* constants (mode constants like S_IRUSR)

char *commands[] = {"ls", "cd", "wc", "exit"};

int parse_line(char *input, char *tokens[], char *delim) {
    int i = 0;
    char *tok = strtok(input, delim);

    while(tok != NULL) {
        tokens[i] = tok;
        i++;
        tok = strtok(NULL, delim);
    }
    tokens[i] = NULL;
    return i;
}

enum command_type get_command_type(char *command) {
    int i = 0;

    for (i=0; i<ERROR; i++) {
        if (strcmp(command, commands[i]) == 0) {
            return i;
        }
    }

	return ERROR;
}

char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
}

// def function for redirection
void handle_redirection(char *args[]) {
    int i = 0;
    int fd;
    while (args[i] != NULL) {
        // output redirection >
        if (strcmp(args[i], ">") == 0) {  
            args[i] = NULL; 
            // open file for writing
            fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644); 
            if (fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            // redirect stdout to file
            dup2(fd, STDOUT_FILENO); 
            // close fd
            close(fd); 
            break;  
            // output redirection >>
        } else if (strcmp(args[i], ">>") == 0) { 
            args[i] = NULL; 
            // open file for writing
            fd = open(args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644); 
            if (fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            // redirect stdout to file
            dup2(fd, STDOUT_FILENO); 
            // close fd
            close(fd); 
            // exit
            break;  
        }
        i++;
    }
}


void handle_pipes(char *args[]) {
    // / fd[0] is read end, fd[1] is write end
    int pipefd[2]; 
    // pos of '|' in the args array
    int pipePos = -1; 
    int i = 0;

    // find '|' pos
    while (args[i] != NULL) {
        if (strcmp(args[i], "|") == 0) {
            pipePos = i;
            break;
        }
        i++;
    }

    if (pipePos == -1) {
        // not found
        return; 
    }

    // split on '|'
    args[pipePos] = NULL; 
    char **cmd1 = args; 
    char **cmd2 = &args[pipePos + 1]; 

    // make pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // fork 1st process
    pid_t pid1 = fork();
    if (pid1 == 0) {
        // child process 1:
        // execute cmd1
        // redirect stdout to pipe's write end
        dup2(pipefd[1], STDOUT_FILENO); 
        close(pipefd[0]);
        close(pipefd[1]);
        // not using our regular functions to not make it too complicated 
        // using regular execvp
        execvp(cmd1[0], cmd1);
        perror("execvp cmd1");
        exit(EXIT_FAILURE);
    }

    // fork 2nd process
    pid_t pid2 = fork();
    if (pid2 == 0) {
        // child process 2
        // will execute cmd2
        dup2(pipefd[0], STDIN_FILENO); 
        close(pipefd[1]); 
        close(pipefd[0]); 
        execvp(cmd2[0], cmd2);
        perror("execvp cmd2");
        exit(EXIT_FAILURE);
    }

    // parent !
    // close read 
    close(pipefd[0]); 
    // close write
    close(pipefd[1]); 
    // wait for the 1st child
    waitpid(pid1, NULL, 0); 
    // wait for the 2nd child
    waitpid(pid2, NULL, 0);
}

bool check_for_pipe(char *args[]) {
    for (int i = 0; args[i] != NULL; i++) {
        // check for '|'
        if (strcmp(args[i], "|") == 0) {
            return true; 
        }
    }
    return false; 
}


void execute_cmd(char *args[], char *ls_path, char *wc_path, int *should_run) {
    // no args 
    if (args[0] == NULL) {
        return; 
    }
    // check for pipe
    if (check_for_pipe(args)) {
        handle_pipes(args);
        return;
    }
    // find case 
    enum command_type cmd_type = get_command_type(args[0]);
    // def pid for fork 
    pid_t pid;
    
    switch (cmd_type) {
        // if ls, wc or error we creare a new process 
        case LS:
        case WC:
        case ERROR:
            pid = fork();
            // child 
            if (pid == 0) { 
                // check if we need to redirect the process 
                handle_redirection(args);
                if (cmd_type == LS) {
                    // using ls_path to run our executable ls.o
                    execvp(ls_path, args);
                } else if (cmd_type == WC) {
                    // using ls_path to run our executable wc.o
                    execvp(wc_path, args);
                } else { 
                    // handle generic commands from the user
                    execvp(args[0], args);
                }
                // check for error
                perror("execvp"); 
                exit(EXIT_FAILURE);
                // parent process
            } else if (pid > 0) { 
                // wait for child process
                wait(NULL); 
            } else { 
                // failed fork
                perror("fork");
            }
            break;
        case CD:
            // change directory
            // in this case we use the trivial implementation of chdir
            // without using fork() since we don't need to create a new process
            // but rather stay in the current process
            if (args[1] != NULL) {
                if (chdir(args[1]) != 0) {
                    perror("chdir");
                }
            } else {
                printf("Please specify a directory\n");
            }
            break;
        case EXIT:
            // set the flag to exit main loop
            *should_run = 0; 
            break;
        default:
            break;
    }
}
