#include "LineParser.h"
#include <linux/limits.h>
#include<sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#define BUFFER_SIZE 2048
#define STDIN 0
#define STDOUT 1
#define TERMINATED  -1
#define RUNNING 1
#define SUSPENDED 0

typedef struct process{
    cmdLine* cmd;                   /* the parsed command line*/
    pid_t pid; 		                /* the process id that is running the command*/
    int status;                     /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next;	        /* next process in chain */
} process;

process* shellProcs=NULL;

process* processBuilder(cmdLine* cmd, pid_t pid){
    process* p = malloc(sizeof(struct process)); // memory allocation
    p->cmd=cmd;
    p->pid=pid;
    p->status=RUNNING;
    p->next=NULL;
    return p;
}

void updateProcessStatus(process* process_list, int pid, int status){
  process_list->status=status;
}

void updateProcessList(process **process_list){
    process* curr_process = (*process_list);
    while(curr_process!=NULL){
        int status;
        int w_status=RUNNING;
        int wait = waitpid(curr_process->pid,&status,WNOHANG);
        if(wait!=0){    //status changed
            if(WIFEXITED(status) || WIFSIGNALED(status))
              w_status=TERMINATED;
            else if(WIFSTOPPED(status))
              w_status=SUSPENDED;
            else if(WIFCONTINUED(status))
              w_status=RUNNING;
            updateProcessStatus(curr_process,curr_process->pid,w_status);
        }
        curr_process=curr_process->next;
    }
}

void freeProcessList(process* process_list){
  if(process_list!=NULL){
  	process* temp;
  	process* current= process_list;
  	while(current!=NULL){
  		freeCmdLines(current->cmd);
  		//free(current->cmd);
  		temp = current->next;
  		free(current);
  		current =temp;
  	}
  }
}

void addProcess(process** process_list, cmdLine* cmd, pid_t pid){
	process* toAdd = processBuilder(cmd,pid);
	if(*process_list==NULL){
		*process_list = toAdd;
		
	}
	else{
		process* current = *process_list;
		while(current->next != NULL){
			current = current->next;
		}
		current->next = toAdd;
	}
}

void printProcessList(process** process_list){
	if(process_list!=NULL){
		updateProcessList(process_list);
		process *father=NULL;
		process *current = *process_list;
		if(current!=NULL && current->status==TERMINATED){  //case head is Terminated.
			while(current!=NULL && current->status==TERMINATED && father==NULL){
			printf("cmd : %s  Proccess ID = %d  status = %d\n\n"
				,current->cmd->arguments[0],current->pid,current->status);
			*process_list= current->next;
			freeCmdLines(current->cmd);
			free(current);
			current=*process_list;		
		}
		}
		while(current != NULL){
			printf("cmd : %s  Proccess ID = %d  status = %d\n\n"
				,current->cmd->arguments[0],current->pid,current->status);
		if(current->status==TERMINATED){////////////////need to fixxxxxx
			if(father==NULL){
				father = current->next;
			}
			else
				father->next=current->next;
			//freeCmdLines(current->cmd);
			//free(current);
			current=current->next;
		}
		else{
			father = current;
			current = current->next;
			}
		}
	}
}

void printPath(){
  char path_name[PATH_MAX];
  getcwd(path_name,PATH_MAX);
  fprintf(stdout, "%s>",path_name);
}


void pipeCommands(cmdLine* cmd){
	int p[2];
	int pid1;
	int pid2;
	if (pipe(p) < 0)//error in pipe
        	exit(1);
	pid1=fork();
	if(pid1<0)//error in fork
		exit(1);
	else if(pid1>0){
		close(p[1]);	
	}
	else {
		close(STDOUT);
		dup2(p[1],STDOUT);
		close(p[1]);
		int returnVal = execvp(cmd->arguments[0],cmd->arguments);
		//exit(1);
		if(returnVal<0){
    			perror("Error while executing the command\n");
   	 		exit(1);
  		}
	}
	pid2= fork();
	if(pid2<0)//error in fork
		exit(1);

	else if(pid2 == 0){
		close(STDIN);
		dup2(p[0],STDIN);
		close(p[0]);
		cmd=cmd->next;
		int returnVal = execvp(cmd->arguments[0],cmd->arguments);
		//exit(1);
  		if(returnVal<0){
    			perror("Error while executing the command\n");
   	 		exit(1);
  		}
	}
	else if(pid2>0){
		close(p[0]);
		waitpid(pid1,NULL,0);
		waitpid(pid2,NULL,0);		
	}
	
	//close(p[0]);
	//exit(1);
}


void execute(cmdLine* cmd, int debug){
  int proccessId;
  if(strcmp(cmd->arguments[0],"quit")==0){
  	freeProcessList(shellProcs);
  	freeCmdLines(cmd);
  	_exit(EXIT_SUCCESS);
  }
  else if(strcmp(cmd->arguments[0],"cd")==0){
    if(chdir(cmd->arguments[1])<0)
      perror("Error while executing command cd");
  }
  else if(strcmp(cmd->arguments[0],"procs")==0){
  	printProcessList(&shellProcs);
  }
  else if(cmd->next!=NULL){
  	pipeCommands(cmd);
	}
  else{
	  proccessId=fork();
	  //int status;
	  if(proccessId<0){
	  	fprintf(stderr, "something went wrong, try againe");
	  }
	  else if(!proccessId){
	  	  //////
	      if(cmd->inputRedirect){
        		close(STDIN);
			if(!fopen(cmd->inputRedirect, "r")){
			     perror(strerror(errno));
			     exit(EXIT_FAILURE);
			}
	      }
	      if(cmd->outputRedirect){
		close(STDOUT);
		if(!fopen(cmd-> outputRedirect, "w+")){
		  perror(strerror(errno));
		  exit(EXIT_FAILURE);
		}
	      }
		  int returnVal=execvp(cmd->arguments[0],cmd->arguments);
		  if(returnVal<0){
		    perror("Error while executing the command genral");
		    _exit(EXIT_FAILURE);
		  }
		
	  }
	  else {
	  	addProcess(&shellProcs, cmd , proccessId);
	  	if(cmd->blocking) 
	  		waitpid(proccessId,NULL,0);
	  	
	  }
	  
  if(debug)
      fprintf(stderr, "PID: %d\nExecuting command: %s\n",proccessId,cmd->arguments[0]);
      }
  }
  
  

int main(int argc, char const *argv[]) {
  FILE* input = stdin;
  char buf[BUFFER_SIZE];
  int debug=0;
  for(int i=1;i<argc;i++){
    if((strcmp("-D",argv[i])==0)){
      debug=1;
    }
  }
  while(1){
    printPath();
    fgets(buf,BUFFER_SIZE,input);
    cmdLine* line = parseCmdLines(buf);
    execute(line,debug);
    if(strcmp(line->arguments[0],"cd")==0||strcmp(line->arguments[0],"procs")==0|| line->next!=NULL)
    	freeCmdLines(line);
  }
  return 0;
}
