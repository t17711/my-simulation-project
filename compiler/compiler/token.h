#pragma once
#include <string>
#include <iostream>

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,
#define error(A,B,C)(std::cout<<A<<" " <<B<< " " <<C<<std::endl ) 

#define for_tk_op(tk) \
	tk(op_push)\
	tk(op_pushi)\
	tk(op_pushc)\
	tk(op_pushf)\
	tk(op_pop)\
	tk(op_add)\
	tk(op_sub)\
	tk(op_mul)\
	tk(op_div)\
	tk(op_not)\
	tk(op_and)\
	tk(op_jmp)\
	tk(op_eof)\
	tk(op_pos)\
	tk(op_neg)\
	tk(op_print)\
	tk(op_printc)\
	tk(op_or)\
	tk(op_greater)\
	tk(op_greater_eql)\
	tk(op_less)\
	tk(op_less_eql)\
	tk(op_eql)\
	tk(op_pushb)\
	tk(op_jfalse)\
	tk(op_jtrue)\
	tk(op_dup)\
	tk(op_remove)\
	tk(op_placeholder)\

enum code_tk {
for_tk_op(GENERATE_ENUM)
};

static const char *code_tk_string[] = {
	for_tk_op(GENERATE_STRING)
};

#define for_tk_name(tk2)\
	tk2(TK)\
	tk2(TK_ID)\
	tk2(TK_BEGIN)\
	tk2(TK_END)\
	tk2(TK_INT)\
	tk2(TK_FLOAT)\
	tk2(TK_BOOL)\
	tk2(TK_STRING)\
	tk2(TK_CHAR)\
	tk2(TK_PLUS)\
	tk2(TK_MINUS)\
	tk2(TK_MUL)\
	tk2(TK_DIV)\
	tk2(TK_OPEN)\
	tk2(TK_CLOSE)\
	tk2(TK_GREATER)\
	tk2(TK_LESS)\
	tk2(TK_EQUAL)\
	tk2(TK_QUES)\
	tk2(TK_GREATER_EQUAL)\
	tk2(TK_LESS_EQUAL)\
	tk2(TK_COMPARE)\
	tk2(TK_SEMICOLON)\
	tk2(TK_COLON)\
	tk2(TK_COMMA)\
	tk2(TK_INCLUDE)\
	tk2(TK_DEFINE)\
	tk2(TK_DOT)\
	tk2(TK_EOF)\
	tk2(TK_FOR)\
	tk2(TK_WHILE)\
	tk2(TK_INT_DEF)\
	tk2(TK_CHAR_DEF)\
	tk2(TK_FLOAT_DEF)\
	tk2(TK_BOOL_DEF)\
	tk2(TK_STRING_DEF)\
	tk2(TK_RETURN)\
	tk2(TK_PRINT) \
	tk2(TK_OR)\
	tk2(TK_EQUAL_COMP)\
	tk2(TK_AND)\
	tk2(TK_DO)\
	tk2(TK_TRUE)\
	tk2(TK_FALSE)\
	tk2(TK_NOT)\
	tk2(TK_IF)\
	tk2(TK_ELSE)\
	tk2(TK_SWITCH)\
	tk2(TK_CASE)\
	tk2(TK_DEFAULT)\

	
	enum token_name {
		for_tk_name(GENERATE_ENUM)
	};

	static const char *token_name_string[] = {
		for_tk_name(GENERATE_STRING)
	};

class token
{
public:
	token();
	token(const token_name &);
	token&  operator = (const token& t);
	
	void print();
public:
	token_name name;
	std::string id;
	int int_value;
	int exp;
	float float_value;;
	char char_val;
};

