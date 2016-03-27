#include <string.h>
typedef enum {
	TK_ID, // identifier
	TK_BEGIN, // { open scope
	TK_END, // } end scope
	TK_INT, // INTEGER
	TK_STRING, // string
	TK_CHAR, // char
	TK_PLUS, 
	TK_MINUS
}token_name ;

struct token{
	token_name name;
	char value[32]; // max name length is 32 
} ;

void print_token(struct token tk){
	
	printf("%.*s \t : %i \n",0, tk.value+strlen(tk.value), tk.name);		

}