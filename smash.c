
#define MAXLINE 4096
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "smash.h"
#include "history.h"
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

//*******************************************//
// Exit Status Guide:                        //
// Exit -1 = exit failure                    //
// Exit 0 = normal exit                      //
// Exit 1 = abnormal, but not critical exit  //
// Exit -3 = blank line count                //
// Exit -11 = Printing history               //
// Exit -25 = error creating pipe/forking    //
// Exit -50 = used for exiting program       //
//                                           //
//                                           //
//                                           //
// Exit 99 = Abnormal exit                   //
// Exit -100 = Check for ext  cmd            //
//*******************************************//

int main () {

// Initial Conditions
int historyCount = 1;
setvbuf(stdout,NULL,_IONBF,0);
setvbuf(stdin,NULL,_IONBF,0);
init_history();

/**********************************/
//   Begin Looping Process Here   //
/**********************************/

while (1) { // Start loop
fputs ("$ ", stderr);


// Pointer Arrays for pipes and I/O Redirection for the life of the loop
int exitStatus = 0; // For collecting exitStatus in the loop
char buffer[MAXLINE] = " "; // Initial status of user input buffer
buffer[MAXLINE] = '\0';
char *token;
char *args[100];
char *input = NULL;
char *output = NULL;
char *pipeCommand[5];
int pipeCount = 1;
int newfd;
int inputfd;
int saved_stdout = dup(1);
int saved_stdin = dup(0);
signal(SIGINT, SIG_IGN);


fgets(buffer, MAXLINE-1, stdin); //pull in user command
buffer[strcspn(buffer, "\n")] = '\0'; // Check and remove "\n"

char commandCopy[MAXLINE-1];
strncpy(commandCopy, buffer, MAXLINE-1); //used to keep name of initial command for history

// Check for piped commands and create input/output chars
token = strtok(buffer, "|");
pipeCommand[0] = token;
// Look for other piped commands


while ((token != NULL) && (pipeCount <= 5)) {
    token = strtok(NULL, "|");
    pipeCount++;
    pipeCommand[pipeCount] = token;
}

// Check for redirections
int forkCount = 0;
int argCounter = 0;
int redirectionCheck;



if (pipeCommand[forkCount] != NULL) {
if (strstr(pipeCommand[forkCount], "<") != NULL && strstr(pipeCommand[forkCount], ">") != NULL) {
  pipeCommand[forkCount] = whiteSpaceRemoval(pipeCommand[forkCount]);
  output = strstr(pipeCommand[forkCount], ">") + 1;
  token = strtok(pipeCommand[forkCount], ">");
  input = strstr(pipeCommand[forkCount], "<") + 1;
  token = strtok(pipeCommand[forkCount], "<");
  redirectionCheck = 3;
} else if (strstr(pipeCommand[forkCount], "<") != NULL) {
  input = strstr(pipeCommand[forkCount], "<") + 1;
  token = strtok(pipeCommand[forkCount], "<");
  redirectionCheck = 2;
} else if (strstr(pipeCommand[forkCount], ">") != NULL) {
  output = strstr(pipeCommand[forkCount], ">") + 1;
  token = strtok(pipeCommand[forkCount], ">");
  redirectionCheck = 1;
} else {
  redirectionCheck = 0;
}

// Parse command
token = strtok(pipeCommand[forkCount], " ");
args[0] = token;

/* Grab additional arguments */
while (token != NULL && argCounter < 100) {
  argCounter++;
  token = strtok(NULL, " ");
  args[argCounter] = token;
}

//check for output redirection
if (redirectionCheck == 3 || redirectionCheck == 1) {
  newfd = open(output, O_CREAT|O_TRUNC|O_RDWR, 0644);
  if (newfd < 0) {
    perror("File could not be opened/created");
    exitStatus = -1;
  } else {
    dup2(newfd, 1);
}
}

//check for input redirection
if (redirectionCheck == 3 || redirectionCheck == 2) {
  inputfd = open(input, O_RDONLY);
  if (inputfd < 0) {
    perror("File could not be opened");
    exitStatus = -1;
  } else {
  dup2(inputfd, 0);
}
}
}

// Ensure file didn't fail aleady and run commands if file didn't fail
if (exitStatus == 0) {
exitStatus = executeCommand(args, argCounter); // Check for internal command or pass to external command
}

if ((exitStatus != -50) && (exitStatus != -11) && (exitStatus != -3)) {
// Won't count blank lines in history or history command

    add_history(commandCopy, exitStatus, historyCount);
    historyCount++;
 }

 if (exitStatus == -50 || strcmp(buffer, "exit") == 0) {
   // 50 is  the return value if the user wants to exit
   clear_history();
   exit(0);

  }

 if (exitStatus == -11) {
   // 11 is used for print history
   add_history(commandCopy, 0, historyCount);
   historyCount++;
   print_history();
}
if (redirectionCheck == 3 || redirectionCheck == 1) {
  fflush(stdout);
  dup2(saved_stdout, 1);
}
if (redirectionCheck == 3 || redirectionCheck == 2) {
  dup2(saved_stdin, 0);
}
  output = NULL;
  input = NULL;

  memset(buffer, 0, MAXLINE-1); // Clear previous buffer
  memset(args, 0, MAXLINE-1); // Clear previous buffer
  fflush(stderr);
}
return 0;
}

/*****************************/
//////////////////////////////
//    EXTERNAL FUNCTIONS    //
//////////////////////////////
/****************************/

///////////////////////
/* REMOVE WHITE SPACE*/
///////////////////////

char* whiteSpaceRemoval(char *input) {
  char *output = input;
  int i;
  int o;

  for (i = o = 0; i<strlen(input); i++, o++) {
    if (input[i] != ' '){
        output[o]=input[i];
    } else {
      o--;
    }
  }
  output[o] = '\0';
  return output;
}
