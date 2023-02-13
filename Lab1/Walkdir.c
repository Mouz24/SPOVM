#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

int compare_elements(char** p1, char** p2)
{
	return strcoll(*p1, *p2);
}

void sort_strings(char** array, int nstrings)
{
	qsort(array, nstrings,
		sizeof(char*), compare_elements);
}

unsigned sort(char* inputDir)
{
	DIR* dir = NULL;
	struct dirent* entry = NULL;
	char pathName[PATH_MAX + 1];
	int count = 0;

	char** strarr = malloc(100 * sizeof(char*));
	for (int i = 0; i < 100; i++)
	{
		strarr[i] = malloc((PATH_MAX) * sizeof(char));
	}

	dir = opendir(inputDir);
		
	if (dir == NULL) {
		printf("Error opening %s: %s", inputDir, strerror(errno));
		return 0;
	}

	entry = readdir(dir);

	while (entry != NULL)
	{
		struct stat entryInfo;

		if ((strncmp(entry->d_name, ".", PATH_MAX) == 0) || (strncmp(entry->d_name, "..", PATH_MAX) == 0)) {
			entry = readdir(dir);
			continue;
		}

		(void)strncpy(pathName, inputDir, PATH_MAX);
		(void)strncat(pathName, "/", PATH_MAX);
		(void)strncat(pathName, entry->d_name, PATH_MAX);

		if (lstat(pathName, &entryInfo) == 0) {
			if (S_ISDIR(entryInfo.st_mode)) 
			{
				printf("\t-d %s\n", pathName);

				sort(pathName);
			}

		}

		if (!S_ISDIR(entryInfo.st_mode))
		{
			for (int i = 0; i < strlen(pathName); i++)
			{
				strarr[count][i] = pathName[i];
			}

			count++;
		}

		entry = readdir(dir);
	}

	strarr = realloc(strarr, count * sizeof(char*));

	sort_strings(strarr, count);

	for (int i = 0; i < count; i++)
	{
		printf("\t");

		for (int j = 0; j < PATH_MAX + 1; j++)
		{
			printf("%c", strarr[i][j]);
		}

		printf("\n");
	}
}

unsigned Walkdir(char* inputDir, char* option, int fl)
{
	DIR* dir = NULL;
	struct dirent* entry = NULL;
	char pathName[PATH_MAX + 1];
	
	if (strstr(option, "s") != NULL)
	{
		sort(inputDir);

		return;
	}

	dir = opendir(inputDir);
	if (dir == NULL) {
		printf("Error opening %s: %s", inputDir, strerror(errno));
		return 0;
	}
	
	entry = readdir(dir);
	
	while (entry != NULL) {
		struct stat entryInfo;
		
		if ((strncmp(entry->d_name, ".", PATH_MAX) == 0) || (strncmp(entry->d_name, "..", PATH_MAX) == 0)) {
			entry = readdir(dir);
			continue;
		}

		(void)strncpy(pathName, inputDir, PATH_MAX);
		(void)strncat(pathName, "/", PATH_MAX);
		(void)strncat(pathName, entry->d_name, PATH_MAX);
		
		if (lstat(pathName, &entryInfo) == 0) {
			if (S_ISDIR(entryInfo.st_mode)) {
				if (strstr(option, "d") != NULL || fl == 1) {
					printf("\t-d %s\n", pathName);
				}
				/*else if (strstr(option, "s") != NULL) {
					printf("\t-d %s\n", pathName);
				}*/

				Walkdir(pathName, option, fl);
			}
			else if (S_ISREG(entryInfo.st_mode)) {
				if (strstr(option, "f") != NULL || fl == 1) {
					printf("\t-f %s has %d bytes\n", pathName, (int)entryInfo.st_size);
				}
			}
			else if (S_ISLNK(entryInfo.st_mode)) {
				char buffer[PATH_MAX + 1];
				
				if (readlink(pathName, buffer, PATH_MAX) != -1) {
					if (strstr(option, "l") != NULL || fl == 1) {
						printf("\t-l %s -> %s\n", pathName, buffer);
					}
				}
				else {
					if (strstr(option, "l") != NULL || fl == 1) {
						printf("\t%s -> (invalid symbolic link!)\n", pathName);
					}
				}
			}
		}
		else {
			printf("Error statting %s: %s\n", pathName, strerror(errno));
		}
		
		entry = readdir(dir);
	}
	
	(void)closedir(dir);
	
	return 0;
}


int main(int argc, char** argv)
{
	char option[PATH_MAX];
	char directory[PATH_MAX];
	int fl;
	
	if (argc == 3) {
		strncpy(option, argv[2], PATH_MAX);
		strcpy(directory, argv[1]);
		fl = 0;
	}
	
	if (argc == 1) {
		strcpy(directory, ".");
		fl = 1;
	}
	
	if (argc == 2) {
		if (strstr(argv[1], "-f") != NULL || strstr(argv[1], "-d") != NULL || strstr(argv[1], "-l") != NULL || strstr(argv[1], "-s") != NULL) {
			strncpy(option, argv[1], PATH_MAX);
			strcpy(directory, ".");
			fl = 0;
		}
		else {
			strcpy(directory, argv[1]);
			fl = 1;
		}
	}
	
	Walkdir(directory, option, fl);
	
	return EXIT_SUCCESS;
}