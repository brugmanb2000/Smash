/*
*
* Functions for executing commands
*
*/

int executeCommand(char *args[], int argCounter);
int executeExternalCommand(char *str[]);
void sigintCatchInMain(int sigIntNum);
void printOut(char *fileName);
int checkForRedirection(char* str);
char* whiteSpaceRemoval(char *input);
void sigintCatchInExternal(int sigIntNum);
