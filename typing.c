#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 1000

int main() {
  int pipe_fd[2];
  pid_t pid;

  if (pipe(pipe_fd) == -1) {
    perror("パイプの作成に失敗");
    exit(EXIT_FAILURE);
  }

  pid = fork();
  if (pid == -1) {
    perror("フォークに失敗");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {
    // 子プロセス - file.txtから文字列を読み込む
    close(pipe_fd[0]);
    char str[BUFSIZE];
    FILE *fp = fopen("data.txt", "r");
    while (fgets(str, BUFSIZE, fp) != NULL) {
      write(pipe_fd[1], str, strlen(str));
    }
    close(pipe_fd[1]);
    exit(EXIT_SUCCESS);
  } else {
    // 親プロセス - 文字列を受け取り入力を受け付ける
    close(pipe_fd[1]);
    char str[BUFSIZE];
    int total_correct = 0;
    int total_target_size = 0;
    char *now;
    read(pipe_fd[0], str, BUFSIZE);
    now = strtok(str, "\n");
    while (now != NULL) {  // 改行コードで区切る
      printf("以下の文字列を入力してください: \n%s\n\n", now);
      char input[BUFSIZE];
      fgets(input, BUFSIZE, stdin);
      int correct = 0;
      int target_size = strlen(now) - 1;  // 改行コードを除外
      for (int i = 0; i < target_size; i++) {
        if (now[i] == input[i]) {
          correct++;
        }
      }
      total_correct += correct;
      total_target_size += target_size;
      printf("正解: %d\n正解率: %lf％ \n", correct,
             100 * (double)correct / (double)target_size);
      now = strtok(NULL, "\n");
    }
    close(pipe_fd[0]);
    printf("\n全体の正解率: %lf％ \n",
           100 * (double)total_correct / (double)total_target_size);
  }

  return 0;
}
