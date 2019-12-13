//-----------------------------------------------------------------------------
//
// NAME
// commands.c -- Define prototypes and symbols for using copy.c
//
//-----------------------------------------------------------------------------
#define MAXLINE 4096
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "smash.h"
#include "history.h"
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

/*******************/
// EXECUTE COMMAND //
/*******************/

int executeCommand(char *args[], int argCounter) {
int status = 0;
if (args[0] == NULL) {
return -3;
}

// check for Exit value
for (int i = 0; i < argCounter; i++) {
  if (strcmp(args[i], "exit") == 0 || strcmp(args[i], "exit\n") == 0) {
  return -50; //gives return value of 50.
}
}

// Return History
if (argCounter == 1 && (strcmp(args[0], "history") == 0)) {
return -11; //gives return to skip adding history since it's been added
}

// check for CD arg Value
else if (argCounter == 2 && ((strcmp(args[0], "cd") == 0))) {
  int chdirStatus = (chdir(args[1]));
  if (chdirStatus != 0) {
  perror(args[1]);
  return -1;
  } else {
  return 0;
}

// return for exec command
} else {
 status = executeExternalCommand(args);
}
return status;
}

/****************************/
// EXECUTE EXTERNAL COMMAND //
/****************************/
int executeExternalCommand(char *args[]) {

  int result = 0;
  pid_t child1 = fork();

// check if fork failed
if (child1 < 0) {
  perror("Problem with forking.");
  exit(-25);
}
//child reads this
if (child1 == 0) {
signal(SIGINT, sigintCatchInExternal);
int invalidCommand = execvp(args[0], args);
args = '\0';
if (invalidCommand == -1) {
  fputs("Command not found.\n", stderr);
  fflush(stderr);
  exit(127);
}
exit(invalidCommand);
}

// For parent process
child1 = wait(&result);
if (WIFEXITED(result)) {
    int exitStatus = WEXITSTATUS(result);
    if (exitStatus == 255) {
      return -1;
    }
      return exitStatus;
    } else {
      printf("\n");
      return -1;
    }
}

/****************************************/
//     CTRL + C Handling for External   //
/****************************************/
void sigintCatchInExternal(int sigIntNum) {
  printf("\n");

}
