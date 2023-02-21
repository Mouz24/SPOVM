#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

char* location(char** envp, char* variable)
{
    char* ENV_VAR;

    for (char** env = envp; *env != 0; env++)
    {
        char* thisEnv = *env;
        
        char* ptr = strstr(thisEnv, variable);
        if (ptr != NULL)
        {
            ENV_VAR = ptr;
        }
    }

    char* DIR = strstr(ENV_VAR, "/");

    return DIR;
}

int main(int argc, char** argv, char** envp)
{
    FILE* file;
    char str[50];
   
    fprintf(stdout, "CHILD: Child process is running...\n");
    fprintf(stdout, "Child P_ID: %d\n", getpid());
    fprintf(stdout, "Child Parent P_ID: %d\n", getppid());
    fprintf(stdout, "Name: %s\n", argv[0]);
    fprintf(stdout, "File Name: %s\n", argv[1]);

    file = fopen(argv[1], "r");

    if (NULL == file) {
        printf("file can't be opened \n");
    }

    while (fscanf(file, "%s", str) != EOF) {
        printf("%s=%s\n", str, getenv(str));
    }

    fclose(file);

    exit(0);
}