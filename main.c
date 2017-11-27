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
  char * line = NULL, p;
  size_t linecap = 0;
  ssize_t linelen;
  char * paths[3] = {"/bin","/sbin",NULL};
  int tempInt;
  //Loop for input
  while(1){
    printf("$:");
    linelen = getline(&line, &linecap, stdin);
    stringArray = parseString(line);
    if(stringArray[0] != NULL){
      //quit internal command
      if((strcmp(stringArray[0],"quit")) == 0){
        printf("Quitting...\n");
        break;
      }else if(0){
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
  }
  free(stringArray);
  return 0;
}

//**Need to make this dynamically allocate based on input size
//Parses a given string based on TOKEN_DELIM
char ** parseString(char line[]){
  int maxEntries = 100, iterator = 0;
  char ** charArray = (char **)malloc(sizeof(char *)*maxEntries);
  char * token;
  token = strtok(line,TOKEN_DELIM);
  while(token != NULL){
    //printf("token: %s\n",token); //debug:To show what is being read
    charArray[iterator] = token;
    iterator++;
    if(iterator <= maxEntries){
      token = strtok(NULL,TOKEN_DELIM);
    }else{
      token = NULL;
    }
  }
  return charArray;
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
