#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <help.h>
#define TOKEN_DELIM " \n\r\t\a"

int changeDir(const char* name){
	if(name == NULL){
		chdir("home"); //chdir(getenv("HOME"));
		printf("Directory is now home.\n");
		return 1;	
	}
	else{
		if(chdir(name) == -1){
			printf("There is no directory called: %s\n", name);
			return -1;
		}
	}
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
  /* it1= 0;
  while(1){
    if(finalArray[it1] == NULL){
      break;
    }else{
      printf("s%d: %s\n",it1,finalArray[it1]);
    }
    it1++;
  }
  */
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

//---------------------------------------------------------
//Handles output redirection, returns -1 on failure
int outputRedirect(char ** stringArray, char * path, int redirectPos){
	if(stringArray[redirectPos+1] == NULL){
		//No filename provided
		printf("No filename provided after '>' command.\n");
		return -1;
	}
	//save filename
	/*
	char * filename = (char *)malloc(sizeof(char)*(strlen(stringArray[redirectPos+1])+3));
	strcat(filename,"./");
	strcat(filename,stringArray[redirectPos+1]);
	*/
	char * filename = stringArray[redirectPos+1];
	//cut stringArray at > so we can pass it as an arg[] to execve
	stringArray[redirectPos] = NULL;
	//create string of command appended to path for execve
	char * command = (char *)malloc(sizeof(char)*((int)strlen(path)+(int)strlen(stringArray[0])+2));
	//Command had a weird value set to it's 0 index, so I had to clear the memory with memset
	memset(command,0,sizeof(char)*((int)strlen(path)+(int)strlen(stringArray[0])+2));
	strcat(command,path);
	strcat(command,"/");
	strcat(command,stringArray[0]);
	//env
	char * env[] = { NULL };
	//creating pipe
	int fd[2];
	if(pipe(fd) == -1){
		perror("Pipe");
		return -1;
	}
	//forking
	pid_t cpid;
	char buf;
	if((cpid = fork()) == -1){
		perror("Fork");
		return -1;
	}
	if(cpid == 0){
		//Be childish
		//Going to call execve, need to write to pipe, change stdout to pipe write
		//then close everything
		close(STDOUT_FILENO);
		dup(fd[1]);
		close(fd[0]);
		close(fd[1]);
		wait(NULL);
		execve(command,stringArray,env);
		perror("Execve");
		free(command);
		exit(1);

	}else{
		//Be parental
		//opening filename using open with modes specified
		int filefd;
		char buf;
		mode_t mode = S_IRWXU|S_IRGRP|S_IROTH;
		if((filefd = open(filename,O_WRONLY|O_CREAT|O_TRUNC,mode))== -1){
			perror("Opening File");
			return -1;
		}
		close(fd[1]);
		while(read(fd[0], &buf,1) > 0){
			write(filefd,&buf,1);
		}
		close(fd[0]);
	}

	return 0;
}

//---------------------------------------------------------
//returns position of > in stringArray, -1 if not found
int findString(char ** stringArray,char * string){
	int position = 0, finalPosition = -1;
	while(stringArray[position] != NULL){
		if(strcmp(stringArray[position],string) == 0){
			finalPosition = position;
			break;
		}
		position++;
	}
	return finalPosition;
}
