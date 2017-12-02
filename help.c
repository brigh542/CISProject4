int changeDir(const char* name);



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
