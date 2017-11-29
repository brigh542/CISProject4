#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

#define TOKEN_DELIM " \n\r\t\a"

//These are for reference purposes
//size_t strlen(char * s) -> returns size of s without NULL
//char * strcat(char * dest, char * src) -> append src to dest
//int strcmp(char * s1, char * s2) -> 0 means equal
//char * strtok(char *restrict str, const char *restrict sep) -> splits string
//int execve(const char *filename, char *const argv[], char *const envp[])


char ** parseString(char line[]);
int tryPaths(char ** paths, char * command);

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
      }else if(0){
        //cd internal command
      }else if((tempInt = tryPaths(paths,stringArray[0])) >= 0){
        //system commands
        printf("Found command in %s.\n",paths[tempInt]);
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

//**Need to make this dynamically allocate based on input size
//Parses a given string based on TOKEN_DELIM
char ** parseString(char line[]){
  int maxEntries = 100, iterator = 0;
  char * stringArray[maxEntries];
  char * token;
  token = strtok(line,TOKEN_DELIM);
  while(token != NULL){
    //printf("token: %s\n",token); //debug:To show what is being read
    stringArray[iterator] = token;
    iterator++;
    if(iterator <= maxEntries){
      token = strtok(NULL,TOKEN_DELIM);
    }else{
      token = NULL;
    }
  }
  stringArray[iterator] = NULL;
  //printf("it: %d string: %s\n",iterator,stringArray[iterator]);
  //Need to check for strings --> cm1 "output string with"
  //Cases: find " within string then add them together until another " is found
  char ** finalArray = (char**)malloc(sizeof(char *)* maxEntries);
  int it1 = 0,it2 = 0,it3 = 0,found = 0,locationFound = -1;
  while(1){
    //printf("looped\n");
    if(stringArray[it1] != NULL){
      //printf("w1: %s\n",stringArray[it1]);
      while(1){
        if(stringArray[it1][it2] == '\0'){
          if(found){
            if(locationFound != it1){
              //append current string to original(locationFound)
              char * tempString = (char *)malloc(sizeof(char)*((int)strlen(stringArray[locationFound])+(int)strlen(stringArray[it1])+2));
              strcat(tempString,stringArray[locationFound]);
              stringArray[locationFound] = tempString;
              strcat(stringArray[locationFound]," ");
              strcat(stringArray[locationFound],stringArray[it1]);
            }else if(finalArray[it3+1] == NULL){
              finalArray[it3] = stringArray[it1];
              it3++;
            }
          }else if(!found){
            finalArray[it3] = stringArray[it1];
            it3++;
          }
          it1++;
          it2 = 0;
          break;
        }else{
          if(stringArray[it1][it2] == '\"' && !found){
            //found first "
            found = 1;
            locationFound = it1;
          }else if(stringArray[it1][it2] == '\"'){
            //found second "
            if(locationFound != it1){
              //append current string to original(locationFound)
              char * tempString = (char *)malloc(sizeof(char)*((int)strlen(stringArray[locationFound])+(int)strlen(stringArray[it1])+2));
              strcat(tempString,stringArray[locationFound]);
              stringArray[locationFound] = tempString;
              strcat(stringArray[locationFound]," ");
              strcat(stringArray[locationFound],stringArray[it1]);
              finalArray[it3] = stringArray[locationFound];
              //printf("s%d: %s\n",it3,finalArray[it3]);
              locationFound = -1;
              it3++;
              found = 0;
              it1++;
              it2 = 0;
              break;
            }else{
              found = 0;
              locationFound = -1;
            }
          }
        }
        it2++;
      }
    }else{
      //printf("Exiting masterLoop\n");
      break;
    }
  }
  it1= 0;
  while(1){
    if(finalArray[it1] == NULL){
      break;
    }else{
      printf("s%d: %s\n",it1,finalArray[it1]);
    }
    it1++;
  }
  return finalArray;
}


//Given array of path names, and command name
//-> returns -1 if no path contains command a
//-> returns n >= 0, the position in paths, if command found
int tryPaths(char ** paths, char * command){
  int location = -1;
  DIR *dir_ptr;
  struct dirent *direntp;
  int position = 0, found = 1;
  while(paths[position] != NULL && found){
    if((dir_ptr = opendir(paths[position])) != NULL){
      //opened directory indicated by paths[location]
      while((direntp = readdir(dir_ptr)) != NULL && found){
        if(strcmp(direntp->d_name,command) == 0){
          //command found
          found = 0;
          location = position;
        }
      }
      closedir(dir_ptr);
    }else{
      //couldn't open the directory indicated by paths
      printf("Directory at %s couldn't be accessed.\n",paths[position]);
    }
    position++;
  }
  return location;
}
