#pragma once
#include "token.h"
class execute
{
public:
	execute(char* code);
	~execute();

protected:
	void mul();
	void add();
	void push();
	void pushi();
	void pop();
	void print();

private:
	char* code;
};

