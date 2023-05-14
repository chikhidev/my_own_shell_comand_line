#include <stdio.h>
#include <stdlib.h>
#include <string.h> //for token
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

void imojie() {
  printf("⠀  ⠀ (\\__/)\n");
  printf("     (•ㅅ•)\n");
  printf("  ＿ノヽ ノ＼＿ \n\n");
}

void remove_newline(char *line_input) {
  int b = 0;
  while (line_input[b] != '\0') {
    if (line_input[b] == '\n') {
      line_input[b] = '\0';
      break;
    }
    b++;
  }
}

int handle_exit(char *command, char *line_input, char **args) {
  int comp;

  comp = strcmp(command, "exit");
  if (comp == 0 || comp == 10) {
    printf("Bye\n");
    return 0;
  }
  return 1;
}

void handle_env(char *command, char *line_input, char **args) {
  int comp;
  char **env = environ;

  comp = strcmp(command, "env");
  if (comp == 0 || comp == 10) {
    while (*env) {
      printf("%s\n", *env);
      env++;
    }
  }
}

int main(void) {
  char *line_input = NULL;
  size_t len = 0;
  char *command;
  char *tmp;
  char *env[] = {"MYVAR=hello", "OTHERVAR=world", NULL};
  char **args = malloc(1000 * sizeof(char));
  int i;

  imojie();

  while (1) {
    i = 0;
    printf("\033[0;31m$ \033[0m");

    if (getline(&line_input, &len, stdin) == -1)
      printf("not working\n");

    remove_newline(line_input);

    // tokinize command:
    command = strtok(line_input, " ");

    if (handle_exit(command, line_input, args) == 0) {
      break;
      exit(0);
    }

    handle_env(command, line_input, args);

    while ((args[i] = strtok(NULL, " ")) != NULL)
      i++;

    args[i] = NULL;

    printf("args:\n");
    for (int x = 0; x < i; x++) {
      printf("%s\n", args[x]);
    }

    // fork:
    int chid = fork();
    if (chid == 0) {
      if (execve(command, args, env) == -1)
        printf("problem to execute ur command!\n");
      exit(0);
    }

    wait(NULL);

    if (chid != 0) {
      free(args);
      free(line_input);
    }
  }

  return 0;
}
