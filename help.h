//Helper file for help.c
int changeDir(const char* name);
char ** parseString(char line[]);
int tryPaths(char ** paths, char * command);
int outputRedirect(char ** stringArray, char * path, int redirectPos);
int findString(char ** stringArray, char * string);
