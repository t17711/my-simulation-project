///////////////////////////////////////
/* header for scanners
*/
#ifndef HEADER_H
#define HEADER_H

#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

typedef enum {
	TK_ID, // identifier
	TK_BEGIN, // { open scope
	TK_END, // } end scope
	TK_INT, // INTEGER
	TK_DOUBLE,
	TK_BOOL,
	TK_STRING, // string
	TK_CHAR, // char
	TK_PLUS,
	TK_MINUS,
	TK_MUL,
	TK_DIV,
	TK_OPEN,  // (
	TK_CLOSE, // )
	TK_GREATER, //>
	TK_LESS,	//<
	TK_EQUAL,	//=
	TK_QUES,	//?
	TK_GREATER_EQUAL,	// >=
	TK_LESS_EQUAL,		// <=
	TK_COMPARE,
	TK_SEMICOLON,		// ;	
	TK_COLON, 		// :
	TK_COMMA,		// :
	TK_INCLUDE,
	TK_DEFINE,
	TK_DOT,
	TK_EOF,
	// key words
	TK_FOR,
	TK_WHILE,
	TK_INT_DEF,
	TK_DOUBLE_DEF,
	TK_BOOL_DEF,
	TK_STRING_DEF,
	TK_RETURN
}token_name;


struct token
{
	// int are int
	// decimals are 2 int
	// there is also exp portion as power of 10

	token_name name;
	std::string value; // max name length is 32 
	unsigned long long  dec;
	unsigned long long   val;		// integer val
	int exp;
};

const char*  get_token_name(unsigned tk){
	switch (tk){
	case	TK_ID: // identifier
		return	"TK_ID";
	case	TK_BEGIN: // { open scope
		return 	"TK_BEGIN";
	case	TK_END: // } end scope
		return 	"TK_END";
	case	TK_INT: // INTEGER
		return	"TK_INT";

	case	TK_DOUBLE: // INTEGER
		return	"TK_DOUBLE";

	case	TK_STRING: // string
		return 	"TK_STRING";

	case	TK_BOOL: // string
		return 	"TK_BOOL";

	case	TK_CHAR: // char
		return 	"TK_CHAR";
	case	TK_PLUS:
		return 	"TK_PLUS";

	case	TK_MINUS:
		return 	"TK_MINUS";

	case	TK_MUL:
		return 	"TK_MUL";

	case	TK_DIV:
		return 	"TK_DIV";

	case	TK_OPEN:
		return 	"TK_OPEN";

	case	TK_CLOSE:
		return 	"TK_CLOSE";

	case	TK_GREATER:
		return 	"TK_GREATER";

	case	TK_LESS:
		return 	"TK_LESS";

	case	TK_EQUAL:
		return 	"TK_EQUAL";

	case	TK_QUES:
		return 	"TK_QUES";

	case	TK_GREATER_EQUAL:
		return 	"TK_GREATER_EQUAL";

	case	TK_LESS_EQUAL:
		return 	"TK_LESS_EQUAL";

	case	TK_SEMICOLON:
		return 	"TK_SEMICOLON";

	case	TK_COLON:
		return 	"TK_COLON";

	case	TK_INCLUDE:
		return 	"TK_INCLUDE";

	case	TK_DEFINE:
		return 	"TK_DEFINE";

	case	TK_COMMA:
		return 	"TK_COMMA";

	case	TK_DOT:
		return 	"TK_DOT";

	case	TK_EOF:
		return 	"TK_EOF";

	case	TK_FOR:
		return 	"TK_FOR";

	case	TK_WHILE:
		return 	"TK_WHILE";

	case	TK_INT_DEF:
		return 	"TK_INT_DEF";

	case	TK_DOUBLE_DEF:
		return 	"TK_DOUBLE_DEF";

	case	TK_BOOL_DEF: // string
		return 	"TK_BOOL_DEF";

	case	TK_STRING_DEF:
		return 	"TK_STRING_DEF";

	case	TK_RETURN:
		return 	"TK_RETURN";

	default:
		return 	"NOT_TOKEN";
	}
}
void print_token(token tk){

	printf(get_token_name(tk.name));
	printf("\t");
	int i = 0;
	if (tk.name == TK_ID || tk.name == TK_STRING)
		std::cout << tk.name;

	if (tk.name == TK_INT)
		printf("\t%i", tk.val);

	else if (tk.name == TK_DOUBLE)
	{
		std::cout << tk.val;
		std::cout << tk.dec;
		std::cout << tk.exp;
	}
	std::cout << std::endl;
}
/*

void string_copy(char * to, char* val){
	int i = 0;
	while (to[i])
	{
		i++;
	}
	to[i] = *val;
}
*/

void get_token(char *buffer, int i,  token* token_list, int j);

void get_eof( token* token_list, int j);
void comment(char *buffer, int i,  token* token_list, int j, int type);
void identifier(char *buffer, int i,  token* token_list, int j);
void digit_token(char *buffer, int i,  token* token_list, int j);
void double_token(char *buffer, int i,  token* token_list, int j);
void exp_token(char *buffer, int i,  token* token_list, int j);

void get_string_token(char *buffer, int i,  token* token_list, int j);

void operator_token(char *buffer, int i,  token* token_list, int j);
void include_token(char *buffer, int i,  token* token_list, int j, int type);
//void define_token	(char *buffer,int i,  token* token_list, int j);
void hash_token(char *buffer, int i,  token* token_list, int j);

/////////////////////////////////////////////////////////////
/*header for symbol table*/
void clear(char* array);
void check_keyword_iter( token tk, char *key, int size, token_name x);
void check_keyword( token tk);

/////////////////////////////////////////////////////////
/*header for parser*/

#endif