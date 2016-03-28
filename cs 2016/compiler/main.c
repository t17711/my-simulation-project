
#include <stdio.h>
#include "extra_func_def.c"
#include "symbol_check.c"

#include "scanner.c"
#include "string.h"


int main()
{
	FILE * pFile;
	long int size;
	pFile = fopen ( "test.txt" , "rb" );
	fseek ( pFile ,0, SEEK_END );  // go to end
	size=ftell (pFile); //  get size
	// allocate size
	
	char *buffer = malloc(size+1);
	
	fseek ( pFile , SEEK_SET, 0 );  // go to begining
	fread (buffer,1,size,pFile);
	int t = 0;
	buffer[strlen(buffer)]=EOF;
	while (buffer[t]!=EOF) buffer[t]=toupper(buffer[t++]);
	
	printf("file size is : %ld\n", size);
	
	
	int i =0;
	int j = 0;
	
	struct token token_list[size + 1];

	get_token(buffer,0, token_list , 0);
	
	j = 0;
	while(token_list[j].name!= TK_EOF)
		print_token(token_list[j++]);

	rewind ( pFile );
	fclose (pFile);

    return 0;
}