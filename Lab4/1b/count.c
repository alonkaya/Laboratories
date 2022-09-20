#include "util.h"
#define SYS_EXIT 1
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_SEEK 19
#define STDIN 0
#define STDOUT 1
#define STDERR 2
extern int system_call();

int main (int argc , char* argv[], char* envp[])
{
    char *output;
    int count=0;
    char ch[1];
    int debug=0;
    int outputFileMode=0;
    int inputFileMode=0;
    int inputFD=STDIN;
    int outputFD=STDOUT;
    int isLetter=0;
    if(argc>1){
    	int i;
    	for(i=0;i<argc;i++){
    		if(strcmp(argv[i],"-D")==0){
    			debug=1;
    			}
	 	else if(strncmp(argv[i],"-o",2)==0){
         	   outputFD= system_call(SYS_OPEN, argv[i]+2,1 | 64,0777);
         	   if(outputFD==-1){
         	   	system_call(SYS_WRITE,STDERR,"err recvd on open output",24);
			system_call(SYS_EXIT,0x55,"\n",1);
         	   }
         	   outputFileMode=i;
        }
      		else if(strncmp(argv[i],"-i",2)==0){
            		inputFD= system_call(SYS_OPEN, argv[i]+2,0);
            		if(inputFD==-1){
         	   	system_call(SYS_WRITE,STDERR,"err recvd on open input",23);
			system_call(SYS_EXIT,0x55,"\n",1);            		
            		}
            		inputFileMode=i;
        }
    	}
    }
    if(debug==1){
    system_call(SYS_WRITE,STDERR,"-DBMP-",6);
    system_call(SYS_WRITE,STDERR,"\n",1);
    if(inputFD==0){
    	system_call(SYS_WRITE,STDERR,"STDIN",5);
    	system_call(SYS_WRITE,STDERR,"\n",1);
    }
    else{
        system_call(SYS_WRITE,STDERR,argv[inputFileMode]+2,strlen(argv[inputFileMode]+2));
    	system_call(SYS_WRITE,STDERR,"\n",1);
    }
    if(outputFD==1){
    	system_call(SYS_WRITE,STDERR,"STDOUT",6);
    	system_call(SYS_WRITE,STDERR,"\n",1);
    }
    else{
        system_call(SYS_WRITE,STDERR,argv[outputFileMode]+2,strlen(argv[outputFileMode]+2));
    	system_call(SYS_WRITE,STDERR,"\n",1);
    }
    int readRV;
    int writeRV; 
    readRV = system_call(SYS_READ,inputFD, ch,1);
    	while(readRV>0){
    		system_call(SYS_WRITE,STDERR,"-DBMP-",6);
		system_call(SYS_WRITE,STDERR,"\n",1);
	    	system_call(SYS_WRITE,STDERR,itoa(SYS_READ),1);
		system_call(SYS_WRITE,STDERR,",",1);
		system_call(SYS_WRITE,STDERR,itoa(readRV),strlen(itoa(readRV)));
		system_call(SYS_WRITE,STDERR,"\n",1);
	    	if(ch[0]=='\n'){
	    		if(isLetter==1){
	    			count+=1;
	    		}
	    		output=itoa(count);
			writeRV=system_call(SYS_WRITE,outputFD,output,strlen(output));
			system_call(SYS_WRITE,outputFD,"\n",1);
			system_call(SYS_WRITE,STDERR,"-DBMP-",6);
			system_call(SYS_WRITE,STDERR,"\n",1);
			system_call(SYS_WRITE,STDERR,itoa(SYS_WRITE),1);
			system_call(SYS_WRITE,STDERR,",",1);
			system_call(SYS_WRITE,STDERR,itoa(writeRV),strlen(itoa(writeRV)));
			system_call(SYS_WRITE,STDERR,"\n",1);
			count=0;
			isLetter=0;
			ch[0]='\0';
	    	}
	    	else if(ch[0]!=' '){
	    		if(isLetter==0){
	    			isLetter=1;
	    		}
	    	}
	    	else{
	    		if(isLetter==1){
	    			isLetter=0;
	    			count+=1;
	    		}
	    	}
	    	readRV = system_call(SYS_READ,inputFD, ch,1);
	    }
	}
    else{
	    while(system_call(SYS_READ,inputFD, ch,1)>0){
	    	if(ch[0]=='\n'){
	    		if(isLetter==1){
	    			count+=1;
	    		}
	    		output=itoa(count);
			system_call(SYS_WRITE,outputFD,output,strlen(output));
			system_call(SYS_WRITE,outputFD,"\n",1);
			count=0;
			isLetter=0;
			ch[0]='\0';
	    	}
	    	else if(ch[0]!=' '){
	    		if(isLetter==0){
	    			isLetter=1;
	    		}
	    	}
	    	else{
	    		if(isLetter==1){
	    			isLetter=0;
	    			count+=1;
	    		}
	    	}
	    }
	}
	if(outputFD>0){
	system_call(SYS_CLOSE,outputFD);
	}
	if(inputFD>0){
	system_call(SYS_CLOSE,inputFD);
	}
  return 0;
}

