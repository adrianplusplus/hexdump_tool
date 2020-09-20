#include<stdio.h>
#include <stdbool.h>
#define USAGE "Usage:\n ./xbd -b <filename>\n ./xbd <filename>"
#define CHARS_PER_LINE_IN_HEX 16
#define CHARS_PER_LINE_IN_BIN 6

char* filename = NULL;

int process_arguments(int argc, char* argv[], bool* binary){
	// passing a wrong number of arguments
	if(argc>3 || argc == 1){
		printf("Invalid number of arguments.\n%s", USAGE);
		return -1;
	}
	
	// check if has -b flag when passing 3 arguments or fail
	if(argc == 3){
		if( strcmp(argv[1], "-b") == 0 && strcmp(argv[2],"-b") == 0 ){
			printf("Invalid arguments.\n%s", USAGE);
			return -2;
		}
		*binary = true;
		filename = ( !strcmp(argv[1], "-b")? argv[2]:argv[1] );
	}
	
	// check that arg is not -b
	if(argc == 2){
		if(strcmp(argv[1], "-b") == 0){
			printf("Invalid argument.\n%s", USAGE);
			return -3;
		}
		filename = argv[1];
	}

	return 0;
		
}

void print_bytes(int character){
	int i ;
	int mask = 128;
	for(i = 0; i < 8; i++){
		if(character == '\0'){
			putchar(' ');
			continue;
		}
		int res = character&mask;
		if(res)
			putchar('1');
		else
			putchar('0');
			
		mask>>=1;
	}
}

void print_line(char* line, int line_counter, int chars_per_line, bool in_binary){
	
	printf("%07x: ", line_counter*(chars_per_line-1));
	
	int i;
	// for each chunk of chars_per_line characters
	for(i = 0; i < chars_per_line; i++){
		// reach end of line
		if(line[i] == '\0'){
			// print spaces for the missing characters in the line , 
			// in case the line is shorter than chars_per_line
			while(++i<chars_per_line) in_binary?printf("         "):printf("   ");
			break;
		};
		
		if(in_binary)
			print_bytes(line[i]);
		else
			printf("%02x", line[i]);
		
		//add a space 1 every 2 characters(hex)
		// 1 every character(binary)
		if(!in_binary && i%2) putchar(' ');
		else putchar(' ');
	}
	
	// TODO: print non printable character as periods
	printf(line);
	
	putchar('\n');
}

int main(int argc, char* argv[]){
	
	bool in_binary= false;
	FILE *fp;
	char buffer[17];
	int chars_per_line;
	
	int p_args_result = process_arguments(argc, argv, &in_binary);
	// return if error occourred
	if(p_args_result < 0) return -9999; 
	
	chars_per_line = (in_binary?CHARS_PER_LINE_IN_BIN:CHARS_PER_LINE_IN_HEX) +1;
		
	fp = fopen(filename, "r");
	int counter = 0;
	
	while( fgets(buffer,chars_per_line, fp) ){
		print_line(buffer, counter, chars_per_line, in_binary);
		counter++;
	}
		
	fclose(fp);
	return 0;
}
