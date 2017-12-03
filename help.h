#ifndef __HELP_H__
#define __HELP_H__

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define TOKEN_DELIM " \n\r\t\a"

char *pathName;



//Helper file for help.c

char ** parseString(char line[]);
int tryPaths(char ** paths, char * command);
int outputRedirect(char ** stringArray, char * path, int redirectPos);
int findString(char ** stringArray, char * string);
//-------------------------------------------------------------------
void path();
void subPath(char* temp);
void addPath(char* temp);
void append(char* a, char b);




#endif 
