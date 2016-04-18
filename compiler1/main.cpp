// ConsoleApplication1.cpp : Defines the entry point for the console application.
//



#include "scanner.cpp"

int main()
{
	FILE * pFile;
	pFile = fopen("test.txt", "rb");
	fseek(pFile, 0, SEEK_END);  // go to end
	const long int size = ftell(pFile); //  get size
	// allocate size

	char *buffer;
	buffer= (char*)malloc(size + 1);
	token *token_list;
	token_list = (token*)malloc(size + 1);

	fseek(pFile, SEEK_SET, 0);  // go to begining
	fread(buffer, 1, size, pFile);
	int t = 0;
	buffer[strlen(buffer)] = EOF;
	while (buffer[t] != EOF) buffer[t] = toupper(buffer[t++]);

	//printf("file size is : %ld\n", size);


	int i = 0;
	int j = 0;

	// fill the token list from buffer
	get_token(buffer, 0, token_list, 0);

	/*
	j = 0;
	while(token_list[j].name!= TK_EOF)
	print_token(token_list[j++]);
	*/
	rewind(pFile);
	fclose(pFile);

	return 0;
}

