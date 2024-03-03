#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>

void cd(char* arg){
	// comment for debugging
	// printf("custom cd:");
	
	// check if the path exists
	if(chdir(arg) == -1){
		// print error message
		printf("cd: %s: No such file or directory\n", arg);
	} else {
		
		char cwd[1024];
		if (getcwd(cwd, sizeof(cwd)) != NULL) {
			// print the current working directory
			printf("Current working directory: %s\n", cwd);
		} else {
			// print error message
			perror("getcwd() error");
		}
	}
}

int main(int argc, char** argv){

	if(argc<2){
		printf("Pass the path as an argument\n");
		return 0;
	}
	cd(argv[1]);
	return 0;
}