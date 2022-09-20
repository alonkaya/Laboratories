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
#define DT_UNKNOWN 0
#define DT_FIFO 1
#define DT_CHR 2
#define DT_DIR 4
#define DT_BLK 6
#define DT_REG 8
#define DT_LNK 10
#define DT_SOCK 12
#define DT_WH 14

extern int system_call();
extern void infector();

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
    char type;
    char *prefix;
    int prefixMode=0;
    int infectMode=0;
    int equalPrefix=0;
    char *fileType;
    int typeScreenMode=0;
    int equalType=0;
    if(argc>1){
    	int i;
    	for(i=0;i<argc;i++){
    		if(strcmp(argv[i],"-D")==0){
    			debug=1;
    			}
    		else if(strncmp(argv[i],"-p",2)==0){
    			prefix=argv[i]+2;
    			prefixMode=1;
    		}
    		else if(strncmp(argv[i],"-a",2)==0){
    			prefix=argv[i]+2;
    			infectMode=1;
    		}
    		else if(strncmp(argv[i],"-t",2)==0){
    			fileType=argv[i]+2;
    			typeScreenMode=1;
    		}
    	}
    }
    if(debug==1){
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
	    /*system_call(SYS_WRITE,STDERR,itoa(buf),strlen(buf)); do i need to print it??*/
	    system_call(SYS_WRITE,STDERR,",",1);
	    system_call(SYS_WRITE,STDERR,"arg4= ",6);
	    system_call(SYS_WRITE,STDERR,itoa(8192),strlen(itoa(8192)));
	    system_call(SYS_WRITE,STDERR,"\n",1);
	    
	    i=0;
	    while(count>i){
	    	entp=(ent*)(buf+i);
	    	type = *(buf+i+entp->len-1);

		if(typeScreenMode==1 && type==positive_atoi(fileType)){
			equalType=1;
		}
	    		    	

	    	if(prefixMode==1||infectMode==1){
	    		if(strncmp(prefix , entp->buf, strlen(prefix))==0){
	    			equalPrefix=1;
	    		}
	    	}
	    	
		if(typeScreenMode==1 && equalType==1){
		system_call(SYS_WRITE,STDOUT,entp->buf,strlen(entp->buf));
	    	system_call(SYS_WRITE,STDOUT,"\n",1);
	    system_call(SYS_WRITE,STDERR,"type ",5);
	    system_call(SYS_WRITE,STDERR,type==DT_UNKNOWN ? "unknown":
					type==DT_FIFO ? "FIFO":
					type==DT_CHR ? "char":
					type==DT_DIR ? "directory":
					type==DT_BLK ? "blank":
					type==DT_REG ? "file":
					type==DT_LNK ? "link":
					type==DT_SOCK ? "socket":"???" ,
					type==DT_UNKNOWN ? 7:
					type==DT_FIFO ? 4:
					type==DT_CHR ? 4:
					type==DT_DIR ? 9:
					type==DT_BLK ? 5:
					type==DT_REG ? 4:
					type==DT_LNK ? 4:
					type==DT_SOCK ? 6:3);
	    system_call(SYS_WRITE,STDOUT,"\n",1);
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
		equalType=0;
		}
	    	else if((infectMode==1)&&(equalPrefix==1)){
	    	system_call(SYS_WRITE,STDOUT,entp->buf,strlen(entp->buf));
	    	system_call(SYS_WRITE,STDOUT,"\n",1);
	    	
	    system_call(SYS_WRITE,STDERR,"type ",5);
	    system_call(SYS_WRITE,STDERR,type==DT_UNKNOWN ? "unknown":
					type==DT_FIFO ? "FIFO":
					type==DT_CHR ? "char":
					type==DT_DIR ? "directory":
					type==DT_BLK ? "blank":
					type==DT_REG ? "file":
					type==DT_LNK ? "link":
					type==DT_SOCK ? "socket":"???" ,
					type==DT_UNKNOWN ? 7:
					type==DT_FIFO ? 4:
					type==DT_CHR ? 4:
					type==DT_DIR ? 9:
					type==DT_BLK ? 5:
					type==DT_REG ? 4:
					type==DT_LNK ? 4:
					type==DT_SOCK ? 6:3);
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
	    infector(entp->buf);
	equalPrefix=0;
	    	}
	    	else if((prefixMode==1)&&(equalPrefix==1)){
	    	
		system_call(SYS_WRITE,STDOUT,entp->buf,strlen(entp->buf));
	    	system_call(SYS_WRITE,STDOUT,"\n",1);
	    	
	    system_call(SYS_WRITE,STDERR,"type ",5);
	    system_call(SYS_WRITE,STDERR,type==DT_UNKNOWN ? "unknown":
					type==DT_FIFO ? "FIFO":
					type==DT_CHR ? "char":
					type==DT_DIR ? "directory":
					type==DT_BLK ? "blank":
					type==DT_REG ? "file":
					type==DT_LNK ? "link":
					type==DT_SOCK ? "socket":"???" ,
					type==DT_UNKNOWN ? 7:
					type==DT_FIFO ? 4:
					type==DT_CHR ? 4:
					type==DT_DIR ? 9:
					type==DT_BLK ? 5:
					type==DT_REG ? 4:
					type==DT_LNK ? 4:
					type==DT_SOCK ? 6:3);
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
	equalPrefix=0;	
	    }
	    else if(prefixMode==0 && typeScreenMode==0){
	    	system_call(SYS_WRITE,STDOUT,entp->buf,strlen(entp->buf));
	    	system_call(SYS_WRITE,STDOUT,"\n",1);
	    	
	    system_call(SYS_WRITE,STDERR,"type ",5);
	    system_call(SYS_WRITE,STDERR,type==DT_UNKNOWN ? "unknown":
					type==DT_FIFO ? "FIFO":
					type==DT_CHR ? "char":
					type==DT_DIR ? "directory":
					type==DT_BLK ? "blank":
					type==DT_REG ? "file":
					type==DT_LNK ? "link":
					type==DT_SOCK ? "socket":"???" ,
					type==DT_UNKNOWN ? 7:
					type==DT_FIFO ? 4:
					type==DT_CHR ? 4:
					type==DT_DIR ? 9:
					type==DT_BLK ? 5:
					type==DT_REG ? 4:
					type==DT_LNK ? 4:
					type==DT_SOCK ? 6:3);
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
		i=i+entp->len;
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
	    i=0;
	    while(count>i){
	    	entp=(ent*)(buf+i);
	    	type = *(buf+i+entp->len-1);

		if(typeScreenMode==1 && type==positive_atoi(fileType)){
			equalType=1;
		}
	    	
	    	if(prefixMode==1||infectMode==1){
	    		if(strncmp(prefix , entp->buf, strlen(prefix))==0){
	    			equalPrefix=1;
	    		}
	    	}
		if(typeScreenMode==1 && equalType==1){
		system_call(SYS_WRITE,STDOUT,entp->buf,strlen(entp->buf));
	    	system_call(SYS_WRITE,STDOUT,"\n",1);
	    system_call(SYS_WRITE,STDERR,"type ",5);
	    system_call(SYS_WRITE,STDERR,type==DT_UNKNOWN ? "unknown":
					type==DT_FIFO ? "FIFO":
					type==DT_CHR ? "char":
					type==DT_DIR ? "directory":
					type==DT_BLK ? "blank":
					type==DT_REG ? "file":
					type==DT_LNK ? "link":
					type==DT_SOCK ? "socket":"???" ,
					type==DT_UNKNOWN ? 7:
					type==DT_FIFO ? 4:
					type==DT_CHR ? 4:
					type==DT_DIR ? 9:
					type==DT_BLK ? 5:
					type==DT_REG ? 4:
					type==DT_LNK ? 4:
					type==DT_SOCK ? 6:3);
	    system_call(SYS_WRITE,STDOUT,"\n",1);
		equalType=0;
		}
	    	else if((infectMode==1)&&(equalPrefix==1)){
		system_call(SYS_WRITE,STDOUT,entp->buf,strlen(entp->buf));
	    	system_call(SYS_WRITE,STDOUT,"\n",1);
	    system_call(SYS_WRITE,STDERR,"type ",5);
	    system_call(SYS_WRITE,STDERR,type==DT_UNKNOWN ? "unknown":
					type==DT_FIFO ? "FIFO":
					type==DT_CHR ? "char":
					type==DT_DIR ? "directory":
					type==DT_BLK ? "blank":
					type==DT_REG ? "file":
					type==DT_LNK ? "link":
					type==DT_SOCK ? "socket":"???" ,
					type==DT_UNKNOWN ? 7:
					type==DT_FIFO ? 4:
					type==DT_CHR ? 4:
					type==DT_DIR ? 9:
					type==DT_BLK ? 5:
					type==DT_REG ? 4:
					type==DT_LNK ? 4:
					type==DT_SOCK ? 6:3);
	    system_call(SYS_WRITE,STDOUT,"\n",1);
	    infector(entp->buf);
		equalPrefix=0;	    	
	    	}
	    	else if((prefixMode==1)&&(equalPrefix==1)){

		system_call(SYS_WRITE,STDOUT,entp->buf,strlen(entp->buf));
	    	system_call(SYS_WRITE,STDOUT,"\n",1);
	    system_call(SYS_WRITE,STDERR,"type ",5);
	    system_call(SYS_WRITE,STDERR,type==DT_UNKNOWN ? "unknown":
					type==DT_FIFO ? "FIFO":
					type==DT_CHR ? "char":
					type==DT_DIR ? "directory":
					type==DT_BLK ? "blank":
					type==DT_REG ? "file":
					type==DT_LNK ? "link":
					type==DT_SOCK ? "socket":"???" ,
					type==DT_UNKNOWN ? 7:
					type==DT_FIFO ? 4:
					type==DT_CHR ? 4:
					type==DT_DIR ? 9:
					type==DT_BLK ? 5:
					type==DT_REG ? 4:
					type==DT_LNK ? 4:
					type==DT_SOCK ? 6:3);
	    system_call(SYS_WRITE,STDOUT,"\n",1);
		equalPrefix=0;
	    }
	    else if(prefixMode==0 && typeScreenMode==0){
	    	system_call(SYS_WRITE,STDOUT,entp->buf,strlen(entp->buf));
	    	system_call(SYS_WRITE,STDOUT,"\n",1);
	    system_call(SYS_WRITE,STDERR,"type ",5);
	    system_call(SYS_WRITE,STDERR,type==DT_UNKNOWN ? "unknown":
					type==DT_FIFO ? "FIFO":
					type==DT_CHR ? "char":
					type==DT_DIR ? "directory":
					type==DT_BLK ? "blank":
					type==DT_REG ? "file":
					type==DT_LNK ? "link":
					type==DT_SOCK ? "socket":"???" ,
					type==DT_UNKNOWN ? 7:
					type==DT_FIFO ? 4:
					type==DT_CHR ? 4:
					type==DT_DIR ? 9:
					type==DT_BLK ? 5:
					type==DT_REG ? 4:
					type==DT_LNK ? 4:
					type==DT_SOCK ? 6:3);
	    system_call(SYS_WRITE,STDOUT,"\n",1);
	    }
		i=i+entp->len;
	   }
    }
  return 0;
}

