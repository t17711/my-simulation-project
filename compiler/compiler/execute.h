#pragma once
#include "token.h"
#include "symtab.h"

class execute
{
public:
	execute(char* code, symtab* tab);

public:
	int mul();
	int div();
	int add();
	int sub();
	int push();
	int pushi();
	int pushc();
	int pushf();
	int pop();
	int print();
	int and();
	int not();
	int jmp();
	int eof();
	int pos();
	int neg();
	int printc();

	int or();
	int greater();
	int greater_eql();
	int less();
	int less_eql();
	int eql();
	int pushb();
	int jfalse();
	int jtrue();

	int dup();
	int remove();

public:
	char* code;	
	int ip;
	char* stack;
	int is;
	symtab* table;


};

