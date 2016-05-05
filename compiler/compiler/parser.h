#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include "token.h"
#include <string>
#include <vector>
#include <algorithm>    // std::find
#include "symtab.h"

using namespace std;
class parser
{
public:
	token** token_list;
	int currtoken;
	symtab* stack;
	int ip;

	char* code;

public:
	parser(token** c, int token_size);
	~parser();

	void error(string);
	void get_token();

	void start_prog();

	void var_decl();
	void decl();
	void statements();
	void statment_types();

	void assignment();

	void expression();
	void add_sub();
	void mul_div();
	void value();
	void expression_mul_div();


	void namelist(char t);

	char type();
	void match(token_name t);

	// these all change same array code[];
	void gen_op_code(code_tk code);
	void gen_op_code_var(int addr);
	void gen_address(int i);
	void emmit(string s);

};

