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

int executeCommand(char *str) {
int argCounter = 0;
char buff[MAXLINE];
char *args[100];

for (int i = 0; i < 100; i++) { // Clear pointers
args[i] == NULL;
}

memset(buff, 0, MAXLINE-1); // Clear previous buff


if (strlen(str) == 1) {
return 3;
} else if (strlen(str) >= MAXLINE) {
  strncpy(buff, str, MAXLINE - 1);

} else {
  strncpy(buff, str, strlen(str));
}

// Grab token
char *token = strtok(buff, " ");
args[0] = token;

if (args[0] == NULL) {
return 3;
}

/* Grab additional tokens */
while (token != NULL && argCounter < 100) {
  argCounter++;
  token = strtok(NULL, " ");
  args[argCounter] = token;  
}

// check for Exit value
if (strcmp(args[0], "exit\n") == 0 || (strcmp(args[0], "exit") == 0)) {
return 50; //gives return value of 50. Smash.c recognizes exitStatus 50 as code to unmalloc memory and exit cleanly. 
}

// Return 0 for CD
else if (argCounter == 1 && ((strcmp(args[0], "cd\n") == 0 || (strcmp(args[0], "cd") == 0)))) {
return 0; //gives return 
}

// Return History
else if (argCounter == 1 && ((strcmp(args[0], "history\n") == 0 || (strcmp(args[0], "history") == 0)))) {
return 11; //gives return to skip adding history since it's been added
}


// check for CD arg Value
else if (argCounter == 2 && ((strcmp(args[0], "cd\n") == 0 || (strcmp(args[0], "cd") == 0)))) {
  int chdirStatus = (chdir(args[1]));
  if (chdirStatus != 0) {
  perror(args[1]); 
  return 1;
  } else {
  return 0; 
}
// ignore if nothing is passed through


// print [] of external commands
} else {
for (int i = 0; i < argCounter-1; i++) {
printf("[%d] %s\n", i, args[i]);
}
printf("[%d] %s", argCounter-1, args[argCounter-1]);

return 127; 
}
}
