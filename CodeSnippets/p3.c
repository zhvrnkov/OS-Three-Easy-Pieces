#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

  printf("Hello world (pid:%d)\n", (int) getpid());
  int rc = fork();

  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } if (rc == 0) {
    printf("children (pid:%d)\n", (int) getpid());
    char *myargs[3];
    myargs[0] = "wc"; // in the source code there is strdup call, which is just copy (?) as i know.
    myargs[1] = "p3.c";
    myargs[2] = NULL;
    execvp(myargs[0], myargs); // also don't clear understand this part. If first parameter in execvp is command, which we want to execute and second parameter is arguments for this command, then why we pass full array, instead of `myargs[1]`?
    printf("This shouldn't print out"); // interesing part. Should explain it in synops.
    
  } else {
    wait(NULL);
    printf("parent (pid:%d) of (pid:%d)\n", (int) getpid(), rc);
  }

  return 0;
  
}
