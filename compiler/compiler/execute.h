#pragma once
#include "token.h"
#include "symtab.h"
#include <vector>
#define max_funct 14


class execute
{
public:
	execute(char* code, symtab* tab);
	~execute();

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


public:
	char* code;	
	int ip;
	char* stack;
	int is;
	symtab* table;


};

