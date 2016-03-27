
#include <stdio.h>
#include "extra_func_def.c"
#include "scanner.c"

int main()
{
	FILE * pFile;
	long int size;
	pFile = fopen ( "test.txt" , "r+" );
	fseek ( pFile ,0, SEEK_END );  // go to end
	size=ftell (pFile); //  get size
	// allocate size
	
	char *buffer = malloc(size+1);
	
	fseek ( pFile , SEEK_SET, 0 );  // go to begining
	fread (buffer,1,size,pFile);

	printf("file size is : %ld\n", size);
	
	// add eof to string
	int j1=strlen(buffer);

	int i1 = 0;
	buffer[j1] = '\0';
	// end at null character now
	
	int i =0;
	int j = 0;
	
	struct token token_list[j1 + 1];

	scan(buffer,0, token_list , 0);
	
	
	rewind ( pFile );
	fclose (pFile);

    return 0;
}