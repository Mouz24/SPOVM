#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

char* concat(char* p1, char* p2)
{   
    char* new_str = malloc(sizeof(char) * (strlen(p1) + strlen(p2)));

    for (int i = 0; i < strlen(p1); i++)
    {
        new_str[i] = p1[i];
    }

    for (int i = strlen(p1), j = 0; i < strlen(new_str), j < strlen(p2); i++, j++)
    {
        new_str[i] = p2[j];
    }

    return new_str;
}

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

extern char** environ;

int main(int argc, char** argv, char** envp)
{
    fprintf(stdout, "PARENT: Parent process is running...\n");
    
    int status;
    pid_t pid;
    
    char** s = environ;

    char* XX_str = malloc(sizeof(char) * 2);
    int XX = 1;

    for (char** env = envp; *env != 0; env++)
    {
        char* thisEnv = *env;
        fprintf(stdout, "%s\n", thisEnv);
    }

    char c = getchar();
    while (c != 'q')
    {
        argv[0] = "child_";
        sprintf(XX_str, "%d", XX);
        argv[0] = concat(argv[0], XX_str);

        if (c  == '+')
        {
            XX++;
            pid = fork();
            if (pid == -1) {
                fprintf(stdout, "PARENT: Error - %d\n", errno);
            }
            if (pid == 0){
                execve(getenv("CHILD_PATH"), argv, envp);
            }
        }
        else if (c == '*')
        {
            XX++;
            pid = fork();
            if (pid == -1) {
                fprintf(stdout, "PARENT: Error - %d\n", errno);
            }
            if (pid == 0) {
                execve(location(envp, "CHILD_PATH"), argv, envp);
            }
        }
        else if (c == '&')
        {
            XX++;
            pid = fork();
            if (pid == -1) {
                fprintf(stdout, "PARENT: Error - %d\n", errno);
            }
            if (pid == 0) {
                execve(location(environ, "CHILD_PATH"), argv, envp);
            }
        }

        c = getchar();
    }
 
    wait(&status);
    
    fprintf(stdout,"PARENT: Child process exited with code %d\n", status);
    fprintf(stdout,"PARENT: Parent process is running...\n");
    
    exit(0);
}