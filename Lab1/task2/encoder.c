#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
int main(int argc, char **argv)
{
	bool isD = false;
	bool isPlus = false;
	bool isNeg = false;
	char *shift_string;
	FILE* file = stdin;
	FILE *out = stdout;
	for(int i = 1; i<argc; i++){
		if(strcmp(argv[i], "-D") == 0) isD = true;
		if (strncmp(argv[i], "+e", 2) == 0){
			isPlus = true;
			shift_string = argv[i] + 2;
		}
		else if (strncmp(argv[i], "-e", 2) == 0){
			isNeg = true;
			shift_string = argv[i] + 2;
		}
		if(strncmp(argv[i], "-i", 2) == 0){
			file = fopen(argv[i]+2, "r");
			if (file == NULL){
                        	printf("%s", "can't open file \n");
                 	        return 0;
                	}	
		}
		if(strncmp(argv[i], "-o", 2) == 0){
			out = fopen(argv[i]+2, "w");
		
		}
	}


	if(isD){
	
		
		char c1;
		int converted = 0;
		while((c1 = fgetc(file)) != EOF){
			int c = (int)c1;
			if(c == '\n'){
				fprintf(out, "\nThe number of letters: %d\n", converted);
				converted = 0;
			}
			else if(c >= 'A' && c <= 'Z'){
				converted = converted + 1;
				fprintf(stderr, "%d %d\n", c, 46);
				fprintf(out, "%c", '.');
			}else{
				fprintf(stderr, "%d %d\n", c, c);
				fprintf(out, "%c", c);
			}
		
			
		}		
		return 0;


	}
	if(isPlus){
		int shift;
		if(*shift_string >= 'A') shift =(int)(*shift_string - 'A' + 10);
		else shift = atoi(shift_string);

		char first = fgetc(file);
		char c = first;
		while(c != EOF){
			while(c != '\n'){
				fprintf(out, "%c", c);
				c = fgetc(file);
			}
			for(int i=0; i<shift; i++) fprintf(out, "%c", first);

			fprintf(out, "%c", '\n');
			if(c == '\n'){
                                first = fgetc(file);
                                c = first;
                        }

		}
	}else if(isNeg){
		int shift;
		if(*shift_string >= 'A') shift =(int)(*shift_string - 'A' + 10);
		else shift = atoi(shift_string);
		
		char first = fgetc(file);
		char c = first;
		while(c != EOF){
			for(int i = 0; i < shift; i++){
				if(c == '\n'){
					fprintf(out, "%s", "incorrect input \n");
					return 0;
				}
				c = fgetc(file);
			}
			while(c != '\n'){
				fprintf(out, "%c", c);
				c = fgetc(file);
			}

			fprintf(out, "%c", '\n');
			if(c == '\n') c = fgetc(file);
		}
	}

	////////// 1a: ////////
	else{
		char c;
		do{
			c = fgetc(file);
			if(c >= 'A' && c <= 'Z')
				fprintf(out, "%c", '.');
			else
				fprintf(out, "%c", c);
		}while(c != EOF);
	}

	return 0;
}


