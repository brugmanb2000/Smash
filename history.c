//-----------------------------------------------------------------------------
//
// NAME
//-----------------------------------------------------------------------------
//
// NAME
// history.c
//
//-----------------------------------------------------------------------------
#include "history.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define MAXLINE 4096

Cmd history[10];
char *commands[10];
int commandCount = 0;
int commandCountBoolean = 0; // Tracks 1 if commandCount has reached 10 once

int historyNum = 0;

void init_history(void) { //Builds data structures for recording cmd history

for (int i = 0; i < MAXHISTORY; i++) {
Cmd command;
history[i] = command;
}
}


void add_history(char *cmd, int exitStatus, int historyNum){ // Adds an entry to the history

if (commandCount == 10) {
commandCount = 0;
commandCountBoolean = 1;
}


// Update History Array with new Command
for (int i = 8; i >=0; i--) {
history[i+1] = history [i];
}

// Clear and copy
memset(history[0].cmd, 0, MAXLINE);
if (strlen(cmd) > MAXLINE) {
  strncpy(history[0].cmd, cmd, MAXLINE - 1);
} else {
  strncpy(history[0].cmd, cmd, strlen(cmd));
}


 // clear previous history[0]

history[0].exitStatus = exitStatus;
history[0].historyInt = historyNum;
historyNum++;
commandCount++;
//printf("%d [%d] %s\n", history[0].historyInt, history[0].exitStatus, history[0].cmd);
//printf("%d [%d] %s\n", history[1].historyInt, history[1].exitStatus, history[1].cmd);
}

void clear_history(void) { //Frees all Malloc'd memory in the history

for (int i = 0; i < 10; i++) { //Clear the malloc's
Cmd command;
history[i] = command;
}
}
void print_history(void) { //Prints the history to stdout


if (commandCountBoolean == 1) {
 int i = 9;
 while (i != -1) {
 printf("%d", history[i].historyInt);
 printf(" [%d] ", history[i].exitStatus);
 printf("%s\n", history[i].cmd);
  i--;
}
} else {
int i = commandCount-1;
 while (i != -1) {
 printf("%d", history[i].historyInt);
 printf(" [%d] ", history[i].exitStatus);
 printf("%s\n", history[i].cmd);
 i--;
 }
 }
 }
