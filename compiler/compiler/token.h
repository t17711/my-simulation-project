#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>

typedef enum {
	TK,
	TK_ID, // identifier
	TK_BEGIN, // { open scope
	TK_END, // } end scope
	TK_INT, // INTEGER
	TK_FLOAT,
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
	TK_FLOAT_DEF,
	TK_BOOL_DEF,
	TK_STRING_DEF,
	TK_RETURN
}token_name;

class token
{
public:
	token();
	token(const token_name &);
	token(const token_name &, const int&);
	token(const token_name&, const std::string&);
	token(const token_name&, const int&, const int&);
	token&  operator = (const token& t);
	~token();
	void print();
	std::string get_token_name(token_name tk);

public:
	token_name name;
	std::string id;
	int int_value;
	int exp;
	int float_value;;
};

#endif