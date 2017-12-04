
#include "help.h"

void path(){

	if(pathName == NULL){
		pathName = (char *)malloc(20);
		printf("Empty Pathname. Please use command: path +\n");
	}
	else
		printf("path: %s\n", pathName);
}

void subPath(char* temp)
{
    if(pathName == NULL){
       pathName = (char *) malloc(20);
    }
    int i;
    char f[50] = ":";
    char c[50] = "";
    if (pathName[1] != temp[1]) {
        strcat(c,f);
        strcat(c,temp);
    } else {
        strcat(c,temp);
    }

    if (strstr(pathName, c) != NULL) {
        char *search = strstr(pathName,c);
        char *end = search + strlen(c);
        char beginning[60] = "";
        for (i=0; i<((strlen(pathName) - (strlen(c) + strlen(end)))); i++){
            beginning[i] = pathName[i];
        }
        strcat(beginning,end);
        strcpy(pathName,beginning);
        printf("path: %s\n",pathName);
    } else {
        printf("\nThe string was not found in path.\n");
    }
}


void addPath(char* temp)
{
    if(pathName == NULL){
       pathName = (char *) malloc(20);
    }
    char colon = ':';
    if(strlen(pathName) == 0){
	//path();
    } else {
        append(pathName,colon);
    }
    strcat(pathName,temp);


    printf("path: %s\n",pathName);
}

void append(char* a, char b){
	int length = strlen(a);
	a[length]=b;
	a[length+1]='\0';
}

char ** setPaths(){
	int maxEntries = 1024, iterator = 0;
	char * stringArray[maxEntries];
	char * token;
	char * tempPath = pathName;
	//printf("s:%s\n",tempPath);
	token = strtok(tempPath,TOKEN_DELIM2);
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
	char ** finalString = (char **)malloc(sizeof(char *) * iterator);
	iterator = 0;
	while(stringArray[iterator] != NULL){
		//printf("s2:%s\n",stringArray[iterator]);
		finalString[iterator] = stringArray[iterator];
		iterator++;
	}
	finalString[iterator] = NULL;
	return finalString;
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
            }else if(stringArray[it3+1] == NULL){
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

//----------------------------------------------------------
//Executes command passed with arguments
void executeProcess(char ** stringArray, char * path){
	//create string of command appended to path for execve
	char * command = (char *)malloc(sizeof(char)*((int)strlen(path)+(int)strlen(stringArray[0])+2));
	//Command had a weird value set to it's 0 index, so I had to clear the memory with memset
	memset(command,0,sizeof(char)*((int)strlen(path)+(int)strlen(stringArray[0])+2));
	strcat(command,path);
	strcat(command,"/");
	strcat(command,stringArray[0]);
	//env
	char * env[] = { NULL };
	//forking
	pid_t cpid;
	if((cpid = fork()) == -1){
		perror("Fork");
	}else{
		if(cpid == 0){
			//be childish
			execve(command,stringArray,env);
			perror("Execve");
			free(command);
			exit(1);
		}else{
			wait(NULL);
			fflush(stdout);
		}
	}
}


//---------------------------------------------------------------------------------------------
//pipeline methods


void isPipeline(char *args[]){
	//test to see if |

	/*
	int count =0;
	while(args[count] != NULL){
		printf("%i : %s\n", count, args[count]);
		count++;
	} */



	//declare file descriptors

	int fileDes[2];
	int fileDes2[2];

	int commandAmt =0;
	char *cmd[256];

	pid_t pid;
	int err =-1;
	int end=0;

	//loop variables

	int a =0, b =0, c= 0, d =0;

	//find number of commands

	while (args[a] != NULL){
		if(strcmp(args[a], "|") ==0){
			commandAmt++;
		}
		a++;
	}
	commandAmt++;

	//main loop
	while(args[b] != NULL && end !=1){
		c=0;

		while(strcmp(args[b], "|") != 0){
			cmd[c] = args[b];
			b++;
			if(args[b] == NULL){
				end = 1;
				c++;
				break;
			}
			c++;
		}

		cmd[c] = NULL;
		b++;

		//connect inputs and outputs

		if(d % 2 != 0){
			//odd
			pipe(fileDes);
		}else{
			//even
			pipe(fileDes2);
		}

		pid = fork();
		if(pid == -1){
			if( d != commandAmt -1){
				if(d % 2 != 0){
					close(fileDes[1]);

				}else{
					close(fileDes2[1]);
				}

			}
			printf("Child Process was not created.\n");
			return;
		}
		if(pid ==0){
			//first command
			if(d == 0){
				dup2(fileDes2[1], STDOUT_FILENO);

			}

			else if(d == commandAmt -1){
				if(commandAmt % 2 !=0){
					dup2(fileDes[0], STDIN_FILENO);
				}
				else{
					dup2(fileDes2[0], STDIN_FILENO);

				}
			}else{
				if( d %2 != 0){
					dup2(fileDes2[0], STDIN_FILENO);
					dup2(fileDes[1], STDOUT_FILENO);
				}
				else{
					dup2(fileDes[0], STDIN_FILENO);
					dup2(fileDes2[1], STDOUT_FILENO);

				}

			}

			if(execvp(cmd[0], cmd) == err){
				kill(getpid(), SIGTERM);

			}

		}

		//closing parent descriptors
		if(d ==0){
			close(fileDes2[1]);
		}
		else if(d == commandAmt-1){
			if(commandAmt % 2 != 0){
				close(fileDes[0]);

			}else{
				close(fileDes2[0]);
			}
		}else{
			if(d %2 != 0){
				close(fileDes2[0]);
				close(fileDes[1]);
			}else{
				close(fileDes[0]);
				close(fileDes2[1]);
			}
		}

		waitpid(pid, NULL, 0);
		d++;

	}

}
