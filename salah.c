#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <unistd.h>
#define BUFFERSIZE 10


int _strlen(char *str)
{
    int i = 0;
    while (str[i])
    {
        i++;
    }
    return (i);
}

int is_new_line(char *str)
{
    if (*str == '\n')
        return (1);
    else
        return (0);
}

int has_space(char *str)
{
    int i = 0;
    while (str[i])
    {
        if (str[i] == ' ')
        {
            return 1;
        }
        i++;
    }
    return 0;
}

int is_empty(char *str)
{
    int i = 0;
    while (str[i])
    {
        if (str[i] != ' ')
        {
            return 0;
        }
        i++;
    }
    return 1;
}

int is_space(char c)
{
    if (c != ' ' && c != '\n' && c != '\t' &&
        c != '\r' && c != '\f')
    {
        return 0;
    }
    return 1;
}

char *l_strip(char *str)
{
    char *striped = malloc(BUFFERSIZE);
    int i = 0, j = 0;
    int is_found_char = 0;

    if (str[0] && !is_space(str[0]))
    {
        is_found_char = 1;
        striped[j] = str[i];
        j++;
        i++;
    }

    while (str[i])
    {
        if (((j + 1) % BUFFERSIZE) == 0)
        {
            striped = realloc(striped, ((j / BUFFERSIZE) + 1) * BUFFERSIZE);
        }

        if (is_found_char)
        {
            striped[j] = str[i];
            j++;
        }
        // we found a character NOT A SPACE
        if (!is_space(str[i + 1]))
        {
            is_found_char = 1;
        }
        i++;
    }
    striped[j] = '\0';
    return striped;
}

// this is the what bro?? ---------------------------------------------
//explain me ----------------------------------------------------------

int executer(char **args, char *env[])
{
    int status;
    pid_t pid;
    
    pid = fork();

    if (pid == 0)
    {
        if (execve(args[0], args, env) == -1) {
            perror("error execve");
            exit(1);
        }
    }
    if (pid == -1)
    {
        perror("error forking");
        exit(1);
    }
    else
    {
        wait(&status);
    }
    return (0);
}

int main(int ac, char **av, char *env[]) {
    // ---------- VARIABLES ----------
    size_t size;
    int i = 0;
    char *cmd = malloc(50);
    char *dir;
    char *path = malloc(50);
    ssize_t bytes;
    char *env_path;
    char *env_path_dup;
    char *execve_env[] = {NULL};
    int arg_len;
    char **args = NULL;
    char *arg = NULL;

    // ---------- READING INPUT ----------
    while (1) {
        printf("> ");
        bytes = getline(&cmd, &size, stdin);

      
        env_path = getenv("PATH");
        env_path_dup = strdup(env_path);

        // ---------- BAD CMD = CONTINUE ----------
        if (is_empty(cmd) || is_new_line(cmd)) {
            continue;
        }
        // else (not empty)  remove spaces from beginning
        // check first character if  =>  /  :
            // get args 
            // execute
        cmd = l_strip(cmd);
        // gettting args :

        // theres 1 always (prog name) 
        arg_len = 1;

        // Allocate dynamically based on the number of arguments
        args = malloc(sizeof(char*) * (arg_len * 2)); 
        // grab cmd and args[0] = cmd =>    args = {"/bin/ls", "-l"}
        args[0] = strtok(cmd, " \n");
        //  save cmd to arg
        arg = args[0]; 

//       --------------- filling args with args ------------------
        
//       ---------- change last from (\n -> \0) ----------
        cmd[bytes - 1] = '\0';

//      ------------ if starts with '/'  ->  cmd run cmd (/bin/pwd) -----------
        if (cmd[0] == '/')
        {
//          cmd = /
            if (cmd[0] == '/' && !cmd[1])
            {
                printf("Permission denied\n");
                exit(1);
            }

            //  args processing
            while (arg != NULL)
            {
                arg = strtok(NULL, " ");
                if (arg && *arg != '\0')
                {
                    args[arg_len] = arg;
                    arg_len++;
                    args = realloc(args, (arg_len + 1) * sizeof(char *));
                }
            }
            args[arg_len] = NULL;

            // executing 
            executer(args, execve_env);
        }
//      if we didnt pass a path to exe ( => cmd like pwd)
        else if (cmd[0] != '/')
        {
//          ---------- processign args ----------
            while (arg != NULL)
            {
                arg = strtok(NULL, " ");
                if (arg && (*arg != '\0'))
                {
                    args[arg_len] = arg;
                    arg_len++;
                    args = realloc(args, (arg_len + 1) * sizeof(char *));
                }
            }
            args[arg_len] = NULL;

//          ---------- taking first dir in PATH (usr/local/bin) ----------
            dir = strtok(env_path_dup, ":");

//          ---------- WHILE THERES MORE ----------
            while ((dir = strtok(NULL, ":")) != NULL)
            {
//              ---------- path for execve (/usr/bin/ls) ----------
                strcpy(path, dir);
                strcat(path, "/");
                strcat(path, cmd);

//              ---------- 1st arg of args => (/usr/bin/ls) ----------
                args[0] = path;

//          arg is always first arg of args, also used to get args like -l ... 
                
//              ---------- if file exists ----------
                if (access(path, F_OK) == 0) {
//                  ---------- executing ----------
//                  ---------- returning 0 means it worked ----------
                    if (executer(args, execve_env) == 0)
//                      ---------- we are breaking so that it only runs once ----------
                        break;
                }
            }
        }
    }

    // ---------- free allocations ----------
    free(path);
    free(cmd);

    return 0;
}
