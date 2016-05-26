#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include "token.h"
#include <string>
#include <vector>
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

	bool proceed = false;
	bool error = false;

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
	void array_decl(char t);

	// got to statement lock here
	void statements();

	// this does single statement
	void statment_types();

	// procedures
	void procedures();
	void procedure_call();

	// this assigns value
	void assignment();
	void array_assign();

	// these are for calculator
	void expression();
	void add_sub();
	void mul_div();
	void value();
	void array_value();
	void expression_mul_div();

	// do while statement
	void do_while();
	void m_while();
	
	// if statement
	void m_if();

	// switch case statement
	void m_switch();

	// this is for print statement
	void print();
	
	// for loop
	void m_for();
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

