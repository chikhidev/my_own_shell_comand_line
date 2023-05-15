#include <stdio.h>
#include <stdlib.h>
#include <string.h> //for token
#include <sys/wait.h>
#include <unistd.h>

void imojie() {
  printf("⠀  ⠀ (\\__/)\n");
  printf("     (•ㅅ•)\n");
  printf("  ＿ノヽ ノ＼＿ \n");
}

int main(void) {
  char *line_input = NULL;
  size_t len = 0;
  char *command;
  char *tmp;
  char *env[] = {"MYVAR=hello", "OTHERVAR=world", NULL};
  char **args = malloc(1000);
  int i = 0;
  int working = 1;

  imojie();
  printf("\n\n");

  while (working) {
    printf("~");
    printf("\033[1;31m");
    printf("$ ");
    printf("\033[0m");

    if (getline(&line_input, &len, stdin) == -1)
      printf("not working\n");

    printf("string: %s\n", line_input);

    // tokinize command:
    command = strtok(line_input, " ");
    printf("command: %s\n", command);
    while ((args[i] = strtok(NULL, " ")) != NULL)
      i++;

    args[i] = NULL;

    if (strcmp(args[0], "exit") == 0) {
      exit(0);
    }

    printf("\n-------------\n");

    // fork:
    int ch_id = fork();
    wait(NULL);

    if (ch_id == 0) {
      if (execve(command, args, env) == -1)
        printf("problem to execute ur command!\n\n");
    }
  }

  // printf("string: %s", line_input);

  return 0;
}
