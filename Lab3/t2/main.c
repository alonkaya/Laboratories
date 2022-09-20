#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct virus {
    unsigned short SigSize;
    unsigned char* sig;
    char virusName[16];
} virus;

typedef struct link {
   struct link *nextVirus;
    virus *vir;
}link;

typedef struct fun_desc {
  char *name;
   void (*fun)(link*);
}fun_desc;

void readVirus(virus * vir, FILE* input){
  char buffer[2];
  int test=0;
  test= fread(buffer, sizeof(char),2, input);
  if(test==2){
    vir->SigSize=buffer[0]+buffer[1]*16*16;
    printf("%d\n", vir->SigSize);
    vir->sig=malloc(vir->SigSize);
    fread(vir->sig, sizeof(char) , vir->SigSize, input);
    fread(vir->virusName, sizeof(char),16,input);
    printf("%s\n", vir->virusName);
  }
}

void printVirus(virus *vir, FILE* output){
  fprintf(output, "virus name: %s\n",vir->virusName);
  fprintf(output, "virus size: %d\n",vir->SigSize);
  fprintf(output, "signature: \n");
  for(int i=0;i<vir->SigSize;i++)
    fprintf(output, "%X ", vir->sig[i]);
  fprintf(output,"\n");
  //fseek(output,0L,SEEK_SET);

}

void list_print(link *virus_list, FILE* output){
  printVirus(virus_list->vir,output);
  if(virus_list->nextVirus!=NULL)
    list_print(virus_list->nextVirus,output);
}

void print_sig(link* vlist){
  //if(vlist->nextVirus!=NULL)//////////////
    list_print(vlist, stdout);
}

link* list_append(link* virus_list, link* to_add){
  //while(virus_list->nextVirus!=NULL){
    //virus_list=virus_list->nextVirus;
  //}
  //virus_list->nextVirus=to_add;
  to_add->nextVirus=virus_list;
  return to_add;
}

void list_free(link *virus_list){
  free(virus_list->vir->sig);
  free(virus_list->vir);
  if(virus_list->nextVirus!=NULL){
    list_free(virus_list->nextVirus);
  }
  free(virus_list);
}

void load_sig(link **vlist){
  printf("please insert file name: \n");
  char fname[20];
  scanf("%s",fname);
  FILE* myFile = fopen(fname, "r");
  int count=0;
  fseek(myFile,0L, SEEK_END);
  count=ftell(myFile);
  fseek(myFile,0L,SEEK_SET);
  while(count){
    virus* v=calloc(1,sizeof(virus));
    readVirus(v,myFile);
    if(v->SigSize>0){
      struct link* l1=malloc(sizeof(link));
      l1->vir=v;
      //l1->nextVirus=NULL;
      *vlist=list_append(*vlist, l1);
      //v->SigSize=0;
      }
   else{
    free(v->sig);
    free(v);
    break;
  }
    count--;
  }
  fclose(myFile);
}

void detect_virus(char *buffer, unsigned int size, link *virus_list){
  //int offS=0;
  link* current = virus_list;
  while(current!=NULL){
    //while(offS<size-current->vir->SigSize){
    for(int i=0;i<size-current->vir->SigSize;i++){
      if(memcmp(current->vir->sig, buffer+i, current->vir->SigSize)==0){
        printf("starting byte of virus: %d\n" , i);
        printf("virus name: %s\n" , current->vir->virusName);
        printf("signature size: %d\n\n" , current->vir->SigSize);
      }
      //offS++;
    }
    current = (current->nextVirus);
  }
}

void detect_v(link *vlist){
  printf("please insert file name\n");
  char fname[20];
  scanf("%s",fname);
  FILE* myFile = fopen(fname, "r");
  char buffer[100000];
  int size=0;
  fseek(myFile,0L, SEEK_END);
  size=ftell(myFile);
  fseek(myFile,0L,SEEK_SET);
  fread(buffer, sizeof(char), 10000, myFile);
  if(size>10000)
    detect_virus(buffer, 10000, vlist);
  else detect_virus(buffer, 10000, vlist);
  fclose(myFile);
}

void kill_virus(char *fileName, int signatureOffset, int signatureSize){
  FILE* myFile = fopen(fileName, "rb+");
  char cut[]={'\x90'};
  fseek(myFile, (long int)signatureOffset,SEEK_SET);
  for(int i=0;i<signatureSize;i++){
      fwrite(cut, sizeof(char), 1, myFile); 
  }
  fclose(myFile);
}

void fix_f(link *vlist){
  printf("please insert file name\n");
  char fname[20];
  scanf("%s",fname);
  //FILE* myFile = fopen(fname, "r");
  printf("please insert location\n");
  int location=0;
  scanf("%i",&location);
  printf("please insert size\n");
  int size=0;
  scanf("%i",&size);
  kill_virus(fname, location, size);
}

int main(int argc, char **argv){
  //char *carray=malloc(5*sizeof(char));
  struct link *vlist=malloc(sizeof(link));
 // strcpy(carray, "");
  vlist->nextVirus=NULL;
  struct fun_desc menu[] = { {"load signatures", load_sig},{"Print signatures", print_sig},{"detect virus", detect_v},{"fix", fix_f},{"quit",list_free},{NULL,NULL}};
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
  if(!(opC>=0&&opC<=6)){
    printf("not withing bounds\n");
    list_free(vlist);
    exit(0);
  }
  printf("within bounds\n");
  menu[opC].fun(vlist);
  if(opC==0){
    link* temp=vlist->nextVirus;
    free(vlist);
    vlist=temp;
  }
  if(opC==4){
    exit(0);
  }

  printf("func=%s\n",menu[opC].name);
}
}
