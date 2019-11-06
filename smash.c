
#define MAXLINE 4096
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "smash.h"
#include "history.h"

int main () {
int historyCount = 1;
int exitStatus = 0; // For collecting exitStatus in the loop
char buffer[MAXLINE]; // ensure buffer is one past the Max Line so no BOA

init_history();
fputs ("$ ", stderr);
while (fgets(buffer, MAXLINE-1, stdin) != NULL) { // while loop for taking in data

exitStatus = executeCommand(buffer);
 
 if ((exitStatus != 3) && (exitStatus != 11)) { // Won't count blank lines in history or history command
    
    add_history(buffer, exitStatus, historyCount); 
    historyCount++;
 }
    
 if (exitStatus == 50) {// 50 is  the return value if the user wants to exit
   clear_history();
   exit(0);
    
  } 
 
 if (exitStatus == 11) {// 11 is used for print history
   add_history(buffer, 0, historyCount);
   historyCount++;
   print_history();
}
  fputs("$ ", stderr);
}
return 0;
}
