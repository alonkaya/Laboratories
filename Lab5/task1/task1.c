#include "LineParser.h"
#include <linux/limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>	
#include <sys/types.h>
#include <sys/wait.h>
#define BUFFER_SIZE 2048
	
void printCWD(){
	char path_name[PATH_MAX];
	getcwd(path_name,PATH_MAX);
	fprintf(stdout, "%s\n",path_name);
}
	
void execute(cmdLine* pCmdLine){
	if(strcmp(pCmdLine->arguments[0],"quit")==0){
		exit(EXIT_SUCCESS);
	}
	if(strcmp(pCmdLine->arguments[0],"cd")==0){
		char* dir = pCmdLine->arguments[1];
		if(chdir(dir)<0)
			fprintf(stderr,"%s\n", "ERROR!");
	}
	int returnVal=execvp(pCmdLine->arguments[0],pCmdLine->arguments);
	if(returnVal<0){
		perror("Could not execute the command");
		exit(EXIT_FAILURE);
	}			
	
	
	
}
		
int main(int argc, char const *argv[]) {
	int flagD = -1;
	for(int i = 0 ; i < argc; i++)
	{
		if(strcmp(argv[i],"-d")==0)
			flagD = 1; 
 
	} 
	FILE* output = stdout;
	if(flagD == 1)
		output = stderr;
	FILE* input = stdin;
	char buf[BUFFER_SIZE];
	while(1){
	printCWD();
	fgets(buf,BUFFER_SIZE,input);
	cmdLine* line = parseCmdLines(buf);
	if(strcmp(line->arguments[0],"quit")==0){
		exit(EXIT_SUCCESS);
	}
	int p = fork();
	if (p==0)
		execute(line);
	else
	{
		if (flagD>0){
			fprintf(output, "PID- %d\n",p);
			fprintf(output, "Executing Command -  %s\n",line->arguments[0]);
		}
		if(line->blocking)
			waitpid(p,NULL,0);	
	} 
		
	freeCmdLines(line);
	}
	return 0;
}
