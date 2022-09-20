
#include <stdio.h>
#include <unistd.h>
#include <elf.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h> 
#include <sys/mman.h>
#include <fcntl.h>

int debbug = 0;
int currentFD = -1 ;
Elf32_Ehdr *ehdr;
void *map;

void toggleDebug(){
	if(debbug == 0 ){
		debbug = 1;
		printf("debbug flag on\n");
		}
	else{
		debbug = 0;
		printf("debbug flag off\n");
	}
}

void examine(){
	printf("insert ELF file name: ");
	char file[20];
	fscanf(stdin,"%s",file);	//taking input
	if(currentFD != -1)
		close(currentFD);	//closing previouse file if needed
	currentFD = open(file, O_RDWR);	//opening current file
	if(currentFD < 0){
		printf("error while opening file");
		exit(-1);
	} 
	lseek(currentFD, 0L, SEEK_END);	// getting file size for map
	size_t size = lseek(currentFD, 0, SEEK_CUR);
	lseek(currentFD, 0L, SEEK_SET);
	printf("size found: %d\n",size);
	if((map = mmap(0,size,PROT_READ | PROT_WRITE , MAP_SHARED, currentFD, 0)) == MAP_FAILED){	//mapping file
		printf("error while maping file");
		exit(-2);
	}
	else{ 
		if(strncmp((char*)(((Elf32_Ehdr *)map))->e_ident,(char*)ELFMAG, 4)!=0){
			printf("not an ELF file!");
			munmap(map,size);	//unmapping
			close(currentFD);
			currentFD=-1;
        }
        
        else{	//case its an ELF file
        printf("mapped\n");
        ehdr=(Elf32_Ehdr *)map;
        printf("Magic:\t %c %c %c\n", ehdr->e_ident[1],ehdr->e_ident[2],ehdr->e_ident[3]);
        printf("Data encoding scheme:\t %s\n",ehdr->e_ident[5]== 0 ? "invalid data encoding":
        					ehdr->e_ident[5]== 1 ? "2's comp little indian":
        					ehdr->e_ident[5]== 2 ? "2's comp big indian":"error");
        printf("Enty point address:\t\t 0x%x\n",ehdr->e_entry);
        printf("section headers offset:\t %d (bytes into file)\n",ehdr->e_shoff);
        printf("Number of section headers:\t %d\n",  ehdr->e_shnum);
        printf("Size of section headers:\t %d (bytes)\n",ehdr->e_shentsize);
        printf("Start of program headers:\t %d (bytes into file)\n",ehdr->e_phoff);
        printf("Number of program headers:\t %d\n",ehdr->e_phnum);
        printf("Size of program headers:\t %d (bytes)\n",ehdr->e_phentsize);
        
        }
      }
    
}
char* getType(int type){
	switch (type) {
        case 0:return "NULL";
        case 1:return "PROGBITS";
        case 2:return "SYMTAB";
        case 3:return "STRTAB";
        case 4:return "RELA";
        case 5:return "HASH";
        case 6:return "DYNAMIC";
        case 7:return "NOTE";
        case 8:return "NOBITS";
        case 9:return "REL";
        case 10:return "SHLIB";
        case 11:return "DYNSYM";
	case 1879048190: return "VERNEED";
	case 1879048191: return "VERSYM";
        default:return "Unknown";
    }
}

void secNames(){
	if(currentFD !=-1){
		Elf32_Shdr* sections_table = (Elf32_Shdr *) (map+ ehdr->e_shoff);	//accessing the sections
		Elf32_Shdr* string_table_sh = &sections_table[ehdr->e_shstrndx]; //stringnames
		const char *const string_table = map + string_table_sh->sh_offset;
		if(debbug){
			fprintf(stderr, "section header string index: %d",ehdr->e_shstrndx);
		}
		printf("[index] Name\tAddr\tOff\tSize\tType\n");
		for(size_t i=0; i< ehdr->e_shnum ; i++){
			//Elf32_Shdr* entry = (sections_table+(i * ehdr->e_shentsize));
			const char* name =string_table + sections_table[i].sh_name;
			if(debbug){
				fprintf(stderr, "name off set %p", map + string_table_sh->sh_offset + sections_table[i].sh_name);
			}
			printf("[%d]\t",i);
			printf("%s\t", name);
			printf("%x\t", sections_table[i].sh_addr);
			printf("%x\t", sections_table[i].sh_offset);
			printf("%d\t", sections_table[i].sh_size);
			printf("%s\n", getType(sections_table[i].sh_type));
		}
	}
	else
		printf("theres no file pending to be handled");
}
void symbols(){
	printf("not implemented yet");
}
void quit(){
	exit(1);
}
struct fun_desc {
  char *name;
  void (*fun)();
};
int main(int argc, char **argv){
  struct fun_desc menu[] = { {"toggle debug mode",toggleDebug},{"examine ELF file",examine},{"print section names",secNames},{"print symbols", symbols},{"quit",quit},{NULL,NULL}};
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
  if(!(opC>=0&&opC<=4)){
    printf("not withing bounds\n");
    exit(0);
  }
  printf("within bounds\n");
  menu[opC].fun();
  printf("func=%s\n",menu[opC].name);
}
}
