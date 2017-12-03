
#include "help.h"

int main(){
  char ** stringArray;
  char * line = NULL;
  size_t linecap = 0;
  ssize_t linelen;
  char * paths[3] = {"/bin","/sbin",NULL};
  int tempInt;
  int * pipeLocations;
  int * redirectLocations;
  //Loop for input
  while(1){
    printf("$:");
    linelen = getline(&line, &linecap, stdin);
    stringArray = parseString(line);
    if(stringArray[0] != NULL){
      //quit internal command
      // pipeLocations = pipeFinder(stringArray);
      // redirectLocations = redirectFinder(stringArray);
      if((strcmp(stringArray[0],"quit")) == 0){
        printf("Quitting...\n");
        break;
      }else if(0){
        //Path internal command
        //--You can ignore this I made it for reference--
        // int pid;
        // if((pid = fork()) == 0){
        //   printf("Child: ");
        //   printf("%s\n",stringArray[0]);
        //   break;
        // }else{
        //   int one = 1;
        //   wait(&one);
        //   printf("Parent: ");
        //   printf("%s\n",stringArray[0]);
        // }
        //------------------------------------------------
      }//else if(0){
        //cd internal command
        else if (strcmp(stringArray[0],"cd") == 0) {

		if(stringArray[1] != NULL){
			if(chdir(stringArray[1]) == -1)
				printf("No such directory: %s\n",stringArray[1]);
			else
				printf("Working directory has been changed to %s.\n", stringArray[1]);
		}
		else{
			if(chdir(getenv("HOME"))==-1){
				printf("Error when trying to change directory to HOME.\n");
			}
    
		}
     }else if(strcmp(stringArray[0],"path") == 0){
		printf("string array 1 = %s\n", stringArray[1]);
		if(stringArray[1] != NULL){
			//printf("string 1 is not null\n");
			if(strcmp("+", stringArray[1])==0){
				//printf("+\n");
				if(stringArray[2] != NULL){
					printf("string 2: %s\n",stringArray[2]);
					addPath(stringArray[2]);
				}else
					printf("ERROR - Missing Path.\n");
			}
			else if(strcmp("-", stringArray[1])==0){
				//printf("-\n");
				if(stringArray[2] != NULL){
					printf("string 2: %s\n",stringArray[2]);
					subPath(stringArray[2]);
				}
				else
					printf("ERROR - Missing Path.\n");
			}
		}
		else
			path();
	
/*
	//empty string array
	int length = strlen(*stringArray);

	if(length){
		for(int i=0;i<length;i++){
			strcpy(stringArray[i], "");
		}		
	}
//	strcpy(stringArray[0], "\0");
	printf("new string array%s\n", stringArray[0]);	
	*/


      }else if((tempInt = tryPaths(paths,stringArray[0])) >= 0){
        //system commands
        //printf("Found command in %s.\n",paths[tempInt]);
	int outputPos,pipePos;
      	if((outputPos = findString(stringArray,">")) >= 0){
      		//Has > so call outputRedirect
      		//printf("Found > at %d.\n",outputPos);
      		outputRedirect(stringArray,paths[tempInt],outputPos);
      	}else if((pipePos = findString(stringArray,"|")) >= 0){
      		//Doesn't have > check for |
      		printf("Found | at %d.\n",pipePos);

      	}else{
      		//Doesn't have either | nor > so normal execute
      		printf("Neither > nor | Found.\n");

      	}
      }else{
        printf("%s :Command not recognized.\n",stringArray[0]);
      }
    }
    free(stringArray);
    linecap = (size_t)0;
  }
  free(stringArray);
  return 0;
}
