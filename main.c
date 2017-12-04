#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include "help.h"

int main(){
  char ** stringArray;
  char * line = NULL;
  size_t linecap = 0;
  ssize_t linelen;
  char ** paths = (char **)malloc(sizeof(char *));
  int tempInt;
  int * pipeLocations;
  int * redirectLocations;
  int outputPos,pipePos;
  //Loop for input
  while(1){
       //free(stringArray);
    fprintf(stdout, "$: ");
    // fflush(stdout);
    linelen = getline(&line, &linecap, stdin);
    stringArray = parseString(line);
    free(paths);
    paths = setPaths();
    if(stringArray[0] != NULL){
      //quit internal command
      if((strcmp(stringArray[0],"quit")) == 0){
        printf("Quitting...\n");
        break;
      }else if (strcmp(stringArray[0],"cd") == 0) {
		    if(stringArray[1] != NULL){
			    if(chdir(stringArray[1]) == -1){
  				printf("No such directory: %s\n",stringArray[1]);
    			}else{
    				printf("Working directory has been changed to %s.\n", stringArray[1]);
          }
        }else{
			       if(chdir(getenv("HOME"))==-1){
				           printf("Error when trying to change directory to HOME.\n");
			       }
        }
   }else if(strcmp(stringArray[0],"path") == 0){
		//printf("string array 1 = %s\n", stringArray[1]);
		if(stringArray[1] != NULL){
			//printf("string 1 is not null\n");
			if(strcmp("+", stringArray[1])==0){
				//printf("+\n");
				if(stringArray[2] != NULL){
					//printf("string 2: %s\n",stringArray[2]);
					addPath(stringArray[2]);
				}else{
					printf("ERROR - Missing Path.\n");
        			}
			}
			if(strcmp("-", stringArray[1])==0){
				//printf("-\n");
				if(stringArray[2] != NULL){
					//printf("string 2: %s\n",stringArray[2]);
					subPath(stringArray[2]);
				}else{
					printf("ERROR - Missing Path.\n");
        			}
			}
  		  }else{
  			path();
        	  }
      
    }else if((tempInt = tryPaths(paths,stringArray[0])) >= 0){
        //system commands
        //printf("Found command in %s.\n",paths[tempInt]);
      	if((outputPos = findString(stringArray,">")) >= 0){
      		//Has > so call outputRedirect
      		//printf("Found > at %d.\n",outputPos);
      		outputRedirect(stringArray,paths[tempInt],outputPos);
      	}else if((pipePos = findString(stringArray,"|")) >= 0){
      		//Doesn't have > check for |
      		//printf("Found | at %d.\n",pipePos);
		        isPipeline(stringArray);
      	}else{
      		//Doesn't have either | nor > so normal execute
		        executeProcess(stringArray,paths[tempInt]);
      	}
      }else{
        printf("%s :Command not recognized.\n",stringArray[0]);
      }
    }
    line = NULL;
    linecap = (size_t)0;
  }
  free(stringArray);
  return 0;
}
