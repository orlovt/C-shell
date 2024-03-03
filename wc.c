#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include<string.h>

void wc(int mode, char* path){
	FILE *file;
	// def counts 
	int lines = 0, words = 0, characters = 0;
	char ch;
	// comment for debugging 
	// printf("custom wc:");
	// check if file exists
	if (path != NULL) {
		file = fopen(path, "r");
		if (file == NULL) {
			printf("Error opening file\n");
			return;
		}
	// if no file check input from stdin
	} else {
		file = stdin;
	}
	// main loop to count 
	while ((ch = fgetc(file)) != EOF) {
		characters++;

		if (ch == '\n') {
			lines++;
		}
		if (ch == ' ' || ch == '\t' || ch == '\n') {
			words++;
		}
	}
	// select mode between lines, words, characters
	if (mode == 1) {
		printf("\t%d\n", lines);
	} else if (mode == 2) {
		printf("\t%d\n", words);
	} else if (mode == 3) {
		printf("\t%d\n", characters);
	} else {
		printf("\t%d", lines);
		printf("\t%d", words);
		printf("\t%d\n", characters);
	}
	// close file if opened
	if (path != NULL) {
		fclose(file);
	}
}

int main(int argc, char** argv){
	if(argc>2){
		if(strcmp(argv[1], "-l") == 0) { 
			wc(1, argv[2]);
		} else if(strcmp(argv[1], "-w") == 0) { 
			wc(2, argv[2]);
		} else if(strcmp(argv[1], "-c") == 0) { 
			wc(3, argv[2]);
		} else {
			printf("Invalid arguments\n");
		}
		
	} else if (argc==2){
		if(strcmp(argv[1], "-l") == 0) { 
			wc(1, NULL);
		} else if(strcmp(argv[1], "-w") == 0) { 
			wc(2, NULL);
		} else if(strcmp(argv[1], "-c") == 0) { 
			wc(3, NULL);
		} else {
			wc(0, argv[1]);
		}
	} else {
		wc(0,NULL);
	}

	return 0;
}