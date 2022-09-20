#include <stdlib.h>
#include <stdio.h>
#include <string.h>
char censor(char c) {
  if(c == '!')
    return '*';
  else
    return c;
}
char* map(char *array, int array_length, char (*f) (char)){
  char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
  /* TODO: Complete during task 2.a */
  for(int i=0;i<array_length;i++){
  mapped_array[i]=f(*(array+i));
  }
  return mapped_array;
}
char encrypt(char c){ /* Gets a char c and returns its encrypted form by adding 2 to its value. 
          If c is not between 0x41 and 0x7a it is returned unchanged */
  if(c>0x41 && c<0x7a)
    return c+2;
  else return c;
}
char decrypt(char c){ /* Gets a char c and returns its decrypted form by reducing 2 to its value. 
            If c is not between 0x41 and 0x7a it is returned unchanged */
  if(c>0x41 && c<0x7a) 
    return c-2;
  else return c;

  }
char dprt(char c){ /* dprt prints the value of c in a decimal representation followed by a 
           new line, and returns c unchanged. */
  printf("c=%d\n",c);
  return c;
  }
char cprt(char c){ /* If c is a number between 0x41 and 0x7a, cprt prints the character of ASCII value c followed 
                    by a new line. Otherwise, cprt prints the dot ('*') character. After printing, cprt returns 
                    the value of c unchanged. */
  if(c>0x41 && c<0x7a) 
    printf("%c\n",c);
  else printf("*\n");
  return c;

}
char my_get(char c){ /* Ignores c, reads and returns a character from stdin using fgetc. */
  c=fgetc(stdin);
  return c;
}
char quit(char c){ /* Gets a char c,  and if the char is 'q' , ends the program with exit code 0. Otherwise returns c. */
  if(c=='q'){
    exit(0);
  }
  return c;
}
struct fun_desc {
  char *name;
  char (*fun)(char);
};
int main(int argc, char **argv){
  char *carray=malloc(5*sizeof(char));
  strcpy(carray, "");
  struct fun_desc menu[] = { {"censor", censor},{"encrypt",encrypt},{"decrypt",decrypt},{"dprt",dprt},{"cprt",cprt},{"my_get",my_get},{"quit",quit},{NULL,NULL}};
  int menu_length=sizeof(menu)/sizeof(menu[0])-1;
  while(1){
  printf("please insert a command number\n");
  for(int i=0;i<menu_length;i++){
  printf("%d:%s\n",i,menu[i].name);
  }
  char n[2];
  n[0]=fgetc(stdin);
  n[1]=fgetc(stdin);
  //printf("%s",n);
  int opC=atoi(n);
  //printf("%d\n",opC);
  if(!(opC>=0&&opC<=menu_length-1)){
    printf("not withing bounds\n\n");
    free(carray);
    exit(0);
  }
  printf("within bounds\n\n");
  char *temp=map(carray,5,menu[opC].fun);
  free(carray);
  carray=temp;
  printf("func=%s\n\n",menu[opC].name);
}
}
