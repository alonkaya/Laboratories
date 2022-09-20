#include "util.h"
#include <dirent.h>
#define SYS_EXIT 1
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_SEEK 19
#define SYS_GETDENTS 141
#define STDIN 0
#define STDOUT 1
#define STDERR 2
# define O_DIRECTORY 4
# define O_FILE	8

extern int system_call();

typedef struct ent{
	int inode;
	int offset;
	short len;
	char buf[1];
} ent;

int main (int argc , char* argv[], char* envp[])
{
    char buf[8192];
    int fd;
    ent *entp;
    int count;
    int i;
    int debug=0;
    if(argc>1){
    	int i;
    	for(i=0;i<argc;i++){
    		if(strcmp(argv[i],"-D")==0){
    			debug=1;
    			}
    	}
    }
    if(debug==1){
    	char type;
    	    fd = system_call(SYS_OPEN,".",0,0777);/*opening the current directory file*/
	    if(fd<0){
	    	system_call(SYS_EXIT,0x55,"\n",1);
	    }
	    system_call(SYS_WRITE,STDERR,"-DBMP-",6);
	    system_call(SYS_WRITE,STDERR,"\n",1);
	    system_call(SYS_WRITE,STDERR,"arg1= ",6);
	    system_call(SYS_WRITE,STDERR,itoa(SYS_OPEN),strlen(itoa(SYS_OPEN)));
	    system_call(SYS_WRITE,STDERR,",",1);
	    system_call(SYS_WRITE,STDERR,"arg2= ",6);
	    system_call(SYS_WRITE,STDERR,".",1);
	    system_call(SYS_WRITE,STDERR,",",1);
	    system_call(SYS_WRITE,STDERR,"arg3= ",6);
	    system_call(SYS_WRITE,STDERR,itoa(0),strlen(itoa(0)));
	    system_call(SYS_WRITE,STDERR,",",1);
	    system_call(SYS_WRITE,STDERR,"arg4= ",6);
	    system_call(SYS_WRITE,STDERR,itoa(0777),strlen(itoa(0777)));
	    system_call(SYS_WRITE,STDERR,"\n",1);
	    
	    count = system_call(SYS_GETDENTS,fd,buf,8192);/* getting directory data*/
	    if(count<=0){
	    system_call(SYS_EXIT,0x55,"\n",1);
	    }
	    
	    system_call(SYS_WRITE,STDERR,"-DBMP-",6);
	    system_call(SYS_WRITE,STDERR,"\n",1);
	    system_call(SYS_WRITE,STDERR,"arg1= ",6);
	    system_call(SYS_WRITE,STDERR,itoa(SYS_GETDENTS),strlen(itoa(SYS_GETDENTS)));
	    system_call(SYS_WRITE,STDERR,",",1);
	    system_call(SYS_WRITE,STDERR,"arg2= ",6);
	    system_call(SYS_WRITE,STDERR,itoa(fd),strlen(itoa(fd)));
	    system_call(SYS_WRITE,STDERR,",",1);
	    system_call(SYS_WRITE,STDERR,"arg3= ",6);
	    system_call(SYS_WRITE,STDERR,itoa(buf),strlen(buf));
	    system_call(SYS_WRITE,STDERR,",",1);
	    system_call(SYS_WRITE,STDERR,"arg4= ",6);
	    system_call(SYS_WRITE,STDERR,itoa(8192),strlen(itoa(8192)));
	    system_call(SYS_WRITE,STDERR,"\n",1);
	    
	    entp=(ent*)(buf+28);
	    count=count-28;
	    i=0;
	    
	    while(count>i){
	    	i=i+entp->len;
	    	entp=(ent*)(buf+i);
	    	type = *(buf+i+entp->len-1);
		system_call(SYS_WRITE,STDOUT,entp->buf,strlen(entp->buf));
	    	system_call(SYS_WRITE,STDOUT,"\n",1);
	    	
	    system_call(SYS_WRITE,STDERR,"type ",6);
	    system_call(SYS_WRITE,STDERR,type==O_DIRECTORY ? "directory":
	    			type==O_FILE? "file":"" ,type==O_DIRECTORY ? 9:
	    			type==O_FILE? 4:0);
	    system_call(SYS_WRITE,STDERR,",",1);
	    system_call(SYS_WRITE,STDERR,"length ",7);
	    system_call(SYS_WRITE,STDERR,itoa(entp->len),strlen(itoa(entp->len)));
	    system_call(SYS_WRITE,STDERR,"\n",1);
	    system_call(SYS_WRITE,STDERR,"-DBMP-",6);
	    system_call(SYS_WRITE,STDERR,"\n",1);
	    system_call(SYS_WRITE,STDERR,"arg1= ",6);
	    system_call(SYS_WRITE,STDERR,itoa(SYS_WRITE),strlen(itoa(SYS_WRITE)));
	    system_call(SYS_WRITE,STDERR,",",1);
	    system_call(SYS_WRITE,STDERR,"arg2= ",6);
	    system_call(SYS_WRITE,STDERR,itoa(STDOUT),strlen(itoa(STDOUT)));
	    system_call(SYS_WRITE,STDERR,",",1);
	    system_call(SYS_WRITE,STDERR,"arg3= ",6);
	    system_call(SYS_WRITE,STDERR,entp->buf,strlen(entp->buf));
	    system_call(SYS_WRITE,STDERR,",",1);
	    system_call(SYS_WRITE,STDERR,"arg4= ",6);
	    system_call(SYS_WRITE,STDERR,strlen(entp->buf),strlen(itoa(strlen(entp->buf))));
	    system_call(SYS_WRITE,STDERR,"\n",1);
		
	    }
    }
    else{
	    fd = system_call(SYS_OPEN,".",0,0777);/*opening the current directory file*/
	    if(fd<0){
	    	system_call(SYS_EXIT,0x55,"\n",1);
	    }
	    count = system_call(SYS_GETDENTS,fd,buf,8192);/* getting directory data*/
	    if(count<=0){
	    system_call(SYS_EXIT,0x55,"\n",1);
	    }
	    entp=(ent*)(buf+28);
	    count=count-28;
	    i=0;
	    while(count>i){
	    	i=i+entp->len;
	    	entp=(ent*)(buf+i);
		system_call(SYS_WRITE,STDOUT,entp->buf,strlen(entp->buf));
	    	system_call(SYS_WRITE,STDOUT,"\n",1);
		
	    }
    }
  return 0;
}

