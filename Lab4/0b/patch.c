#include "util.h"

#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_SEEK 19
#define STDOUT 1
extern int system_call();

int main (int argc , char* argv[], char* envp[])
{

  int err;
  int fd = system_call(SYS_OPEN , argv[1], 1);
  if(fd<0){
  	system_call(1, 0x55 , 0x506, 0);
  }
  err= system_call(SYS_SEEK , fd, 657, 0);
  if(err==0){
  	system_call(1, 0x55 , 0x506, 0);
  }
  err= system_call(SYS_WRITE, fd , argv[2] ,strlen(argv[2]));
    if((err==-1) |(err==0)){
  	system_call(1, 0x55 , 0x506, 0);
  }
  err=system_call(SYS_CLOSE , fd);
    if(fd==-1){
  	system_call(1, 0x55 , 0x506, 0);
  }
  return 0;
}

