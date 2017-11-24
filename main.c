#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#define TOKEN_DELIM " \n\r\t\a"

//These are for reference purposes
//size_t strlen(char * s) -> returns size of s without NULL
//char * strcat(char * dest, char * src) -> append src to dest
//int strcmp(char * s1, char * s2) -> 0 means equal
//char * strtok(char *restrict str, const char *restrict sep) -> splits string

char ** parseString(char line[]);

int main(){
  //char line[] = "Dakotah\rMade This to\ntest spaces";
  char ** stringArray;
  char * line = NULL;
  size_t linecap = 0;
  ssize_t linelen;
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
        //path internal command
      }else if(0){
        //cd internal command
      }else if(0){
        //system commands
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
