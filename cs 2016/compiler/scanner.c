#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void identifier(char *buffer,int i, struct token* token_list, int j){
	
	if (buffer[i]=='\0') return;
	else if(buffer[i] <= 32){
		token_list[j].name = TK_ID;
		print_token(token_list[j]);
		scan(buffer,++i, token_list, ++j);
		return;
	}
	else if (buffer[i]){	
		char curr  = buffer[i];
		printf("%c", curr);
		strcpy(token_list[j].value, &curr);
		identifier(buffer,++i, token_list, j);
		return;
	}
	else {
		printf("error not identifier");
		return;
	}
	
}

int scan(char *buffer, int i, struct token* token_list , int j)
{	
	/*printf("%d\n",strlen(buffer));

	
	while ( buffer[i1] != '\0') 
	{
		printf("%c", buffer[i1]);
		++i1;
	}
*/
LOOP:
	if (buffer[i]=='\0')  return;
	else if (buffer[i] <= 32) {
		i++;
		goto LOOP;
	}
	else //if (*buffer > 32 && *buffer < 122)
	{
			identifier(buffer,i, token_list, j);
	}

	return 0;
	
}

