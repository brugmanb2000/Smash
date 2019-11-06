

//Define the layout of a single entry in the history array
typedef struct {
  char cmd[4096]; //Saved copy of the user's command string
  int exitStatus; //Exit Status from this command
  int historyInt;
} Cmd;

//Define the Maximum Number of Entries in the History Array
#define MAXHISTORY 10

void init_history(void); //Builds data structures for recording cmd history
void add_history(char *cmd, int exitStatus, int historyNum); // Adds an entry to the history
void clear_history(void); //Frees all Malloc'd memory in the history
void print_history(void); //Prints the history to stdout
void history_update(void);

int arrayCount(Cmd *array[]);