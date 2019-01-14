#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int arcgc, char *argv[]) {

  printf("Hello world (pid:%d)\n", (int) getpid());
  int rc = fork();

  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } if (rc == 0) {
    printf("children (pid:%d)\n", (int) getpid());
  } else {
    wait(NULL);
    printf("parent (pid:%d) of (pid:%d)\n", (int) getpid(), rc);
  }

  return 0;
}
