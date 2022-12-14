#include "LineParser.h" 
#include <linux/limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#define BUFFER_SIZE 2048
#define TERMINATED  -1
#define RUNNING 1
#define SUSPENDED 0

//process struct
typedef struct process{
    cmdLine* cmd;                   
    pid_t pid; 		                
    int status;                    
    struct process *next;	       
} process;

process* GProcsList;

void updateProcessList(process **);
void updateProcessStatus(process*,int,int);

process* makeNewProcess(cmdLine* cmd, pid_t pid){
    process* new_process = malloc(sizeof(struct process));
    new_process->cmd=cmd;
    new_process->pid=pid;
    new_process->status=RUNNING;
    new_process->next=NULL;
    return new_process;
}

process* list_append(process* _process, cmdLine* cmd, pid_t pid){
    if(_process==NULL){
        process* new_process = makeNewProcess(cmd,pid);
        _process=new_process;
    }
    else
        _process->next=list_append(_process->next,cmd, pid);
    return _process;
}

//adding new process
void addProcess(process** process_list, cmdLine* cmd, pid_t pid){
  (*process_list)=list_append((*process_list),cmd, pid);
}

char* getStatusString(int status){
    if(status == TERMINATED)
      return "Terminated";
    else if(status == RUNNING)
      return "Running";
    else          
      return "Suspended";
}

void printProcess(process* process){
    char command[100]="";
    if(process!=NULL && process->cmd->argCount>0)
        for(int i=0;i<process->cmd->argCount;i++){
        	const char* argument = process->cmd->arguments[i];
        	strcat(command,argument);
        	strcat(command," ");
    }
    printf("%d       %s       %s\n",process->pid,command,getStatusString(process->status));
}

void list_print(process* process_list){
    process* curr_process = process_list;
    while(curr_process != NULL){
        printProcess(curr_process);
        curr_process=curr_process->next;
    }
}

void delete_process(process* toDelete){
    freeCmdLines(toDelete->cmd);
    toDelete->cmd=NULL;
    toDelete->next=NULL;
    free(toDelete);
    toDelete=NULL;
}

int deleteTerminatedProcesses(process** process_list){
    process* curr_process = *process_list;
    process* prev_process;
    if(curr_process!=NULL && curr_process->status==TERMINATED){
        *process_list=curr_process->next;
        delete_process(curr_process);
	return 1;
    }
    while (curr_process!=NULL && curr_process->status!=TERMINATED){
        prev_process=curr_process;
        curr_process=curr_process->next;
    }
    if(curr_process==NULL)
      return 0;
    else{
        prev_process->next=curr_process->next;
        delete_process(curr_process);
        return 1;
    }
}

//print process list
void printProcessList(process** process_list){
    updateProcessList(process_list);
    printf("PID         Command         STATUS\n");
    list_print((*process_list));
    while(deleteTerminatedProcesses(process_list)){};
}

//free memory
void free_list(process* process_list){
    process* curr_process=process_list;
    if(curr_process!=NULL){
        free_list(curr_process->next);
        freeCmdLines(curr_process->cmd);
        free(curr_process->cmd);
        free(curr_process);
    }
}

//free list
void freeProcessList(process* process_list){
  free_list(process_list);
}


void updateProcessList(process **process_list){
    process* curr_process = (*process_list);
    while(curr_process!=NULL){
        int status;
        int wait = waitpid(curr_process->pid,&status,WNOHANG | WUNTRACED | WCONTINUED);
        if(wait!=0){   
            updateProcessStatus(curr_process,curr_process->pid,status);
        }
        curr_process=curr_process->next;
    }
}


void updateProcessStatus(process* process_list, int pid, int status){
  int w_status=RUNNING;
  if(WIFEXITED(status) || WIFSIGNALED(status))
    w_status=TERMINATED;
  else if(WIFSTOPPED(status))
    w_status=SUSPENDED;
  else if(WIFCONTINUED(status))
    w_status=RUNNING;
  process_list->status=w_status;
}

void printPath(){
  char path_name[PATH_MAX];
  getcwd(path_name,PATH_MAX);
  fprintf(stdout, "%s\n",path_name);
}

//dealing with special command
int specialCommand(cmdLine* command){
  int isSpecial=0;
  if(strcmp(command->arguments[0],"quit")==0){
    isSpecial=1;
    _exit(EXIT_SUCCESS);
  }
  else if(strcmp(command->arguments[0],"cd")==0){
    isSpecial=1;
    if(chdir(command->arguments[1])<0)
      perror("bad cd command");
  }
  else if(strcmp(command->arguments[0],"showprocs")==0){
    isSpecial=1;
    printProcessList(&GProcsList);
  }
  else if(strcmp(command->arguments[0],"stop")==0){
    isSpecial=1;
    int pid = atoi(command->arguments[1]);
    if(kill(pid,SIGINT)==-1)    
      perror(strerror(errno));
    else
      printf("%d handling SIGINT\n",pid);
  }
  else if(strcmp(command->arguments[0],"nap")==0){
    isSpecial=1;
    int time = atoi(command->arguments[1]);
    int pid = atoi(command->arguments[2]);
    int suspend_fork;
    int kill_success;
      if ((suspend_fork = fork()) == 0){
        kill_success = kill(pid, SIGTSTP);
        if (kill_success == -1)
          perror("kill failed");
        else{
          printf("%d handling SIGTSTP\n",pid);
          sleep(time);
          kill_success = kill(pid, SIGCONT);
          if (kill_success == -1)
            perror("kill failed");
          else
            printf("%d handling SIGCONT\n",pid);
        }
        _exit(1);
      }
  }
  if(isSpecial)
    freeCmdLines(command);
  return isSpecial;
}

void execute(cmdLine* pCmdLine, int flagD){
  if(!specialCommand(pCmdLine)){
    pid_t childPid;
    if(!(childPid=fork())){
        if(execvp(pCmdLine->arguments[0],pCmdLine->arguments)<0){
          perror("can't execute the command");
          _exit(EXIT_FAILURE);
      }
    }
    if(childPid!=-1)  
      addProcess(&GProcsList,pCmdLine,childPid);
    if(flagD){
      fprintf(stderr, "PID: %d\nExecuting command: %s\n",childPid,pCmdLine->arguments[0]);
    }
    if(pCmdLine->blocking){   
      waitpid(childPid,NULL,0);
    }
  }
}

int main(int argc, char const *argv[]) {
  FILE* input = stdin;
  char buf[BUFFER_SIZE];
  int flagD=0;
  GProcsList=NULL;
  for(int i=1;i<argc;i++){
    if((strcmp("-D",argv[i])==0)){
      flagD=1;
    }
  }
  while(1){
    printPath();
    fgets(buf,BUFFER_SIZE,input);
    cmdLine* line = parseCmdLines(buf);
    execute(line,flagD);
  }
  return 0;
}
