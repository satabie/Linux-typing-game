#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 100

int main() {
  int pipe_fd[2];
  pid_t pid;

  if (pipe(pipe_fd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {
    // Child process - generate random string
    close(pipe_fd[0]);
    char str[BUFSIZE];
    for (int i = 0; i < BUFSIZE - 1; i++) {
      str[i] = 'a' + rand() % 26;
    }
    str[BUFSIZE - 1] = '\0';
    write(pipe_fd[1], str, strlen(str));
    close(pipe_fd[1]);
    exit(EXIT_SUCCESS);
  } else {
    // Parent process - receive string and take input
    close(pipe_fd[1]);
    char str[BUFSIZE];
    read(pipe_fd[0], str, BUFSIZE);
    printf("Type the following string: %s\n", str);
    char input[BUFSIZE];
    fgets(input, BUFSIZE, stdin);
    int correct = 0;
    for (int i = 0; i < BUFSIZE - 1; i++) {
      if (str[i] == input[i]) {
        correct++;
      }
    }
    printf("You typed %d characters correctly out of %d\n", correct,
           BUFSIZE - 1);
  }

  return 0;
}
