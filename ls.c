#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>

void ls(char *path, bool recurse_flag) {
    DIR *dir;
    struct dirent *entry;

    if (path == NULL) {
        path = ".";
    }
	
	
	// comment for debugging 
    // printf("custom ls:");
    printf("%s:\n", path);
	// opening directory
    dir = opendir(path);
    // if directory is not found
    if (dir == NULL) {
        perror("opendir");
        return;
    }
	// loop for reading the directory
	// error check for the directory
    while ((entry = readdir(dir)) != NULL) {
		// print the name of the file
		// if the file is not . or .. then print the name of the file
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            printf("%s\n", entry->d_name);
			// if the file is a directory and the recurse flag is true then call the ls function again
            if (recurse_flag && entry->d_type == DT_DIR) {
                // Buffer for subpath
				char subpath[1024]; 
                snprintf(subpath, sizeof(subpath), "%s/%s", path, entry->d_name);
                // Separate directories for readability
				printf("\n");  
				// Recurse without level
                ls(subpath, true);  
            }
        }
    }

    closedir(dir);
	// newline for readability
    printf("\n"); 
}

int main(int argc, char *argv[]) {
	if (argc < 2) { // No -R flag and no path name
		ls(NULL, false);
	} else if (strcmp(argv[1], "-R") == 0) {
		if (argc == 2) { // only -R flag
			ls(NULL, true);
		} else { // -R flag with some path name
			ls(argv[2], true);
		}
	} else { // no -R flag but path name is given
		ls(argv[1], false);
	}

	return 0;
}
