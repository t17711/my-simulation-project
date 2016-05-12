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
	void code_print();
	//void error(string);

	// get next token
	token* curr_token();


	// start program here
	void start_prog();

	// do variable declaration here
	void var_decl();
	void decl();
	void namelist(char t);

	// got to statement lock here
	void statements();

	// this does single statement
	void statment_types();

	// this assigns value
	void assignment();

	// these are for calculator
	void expression();
	void add_sub();
	void mul_div();
	void value();
	void expression_mul_div();

	// do while statement
	void do_while();
	void m_while();
	// if statement
	void m_if();
	// this is for print statement
	void print();
	// this gives the type if token is type tokwen
	char type();
	// compares token and moves up
	void match(token_name t);


	// these all change same array code[];
	void gen_op_code(code_tk code);
	void gen_char(char val);
	void gen_float(float, int);
	void gen_address(int i);
	void gen_bool(bool v);

	void emmit(string s);

};

