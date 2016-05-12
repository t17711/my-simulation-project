#pragma warning(disable:4996)

#include "scanner.cpp"
#include "parser.h"
#include "execute.h"

using namespace std;

int main(int argc, char **argv)
{
	scanner c = scanner();
	c.get_token();
	c.print();
	
	parser p = parser(c.token_list, c.j);
	p.start_prog();
	p.stack->print();
	p.code_print();
	
	execute e = execute(p.code, p.stack);
   code_tk t = *(code_tk*)(e.code + e.ip);


typedef int (execute::*Function) ();

// needs to be in order same as enum
   Function exe[] = {
	   &execute::push,
	   &execute::pushi,
	   &execute::pushc,
	   &execute::pushf,
	   &execute::pop,
	   &execute::add,
	   &execute::sub,
	   &execute::mul,
	   &execute::div,
	   &execute::not,
	   &execute::and,
	   &execute::jmp,
	   &execute::eof,
	   &execute::pos,
	   &execute::neg,
	   &execute::print,
	   &execute::printc,
	   &execute::or,
	   &execute::greater,
	   &execute::greater_eql,
	   &execute::less,
	   &execute::less_eql,
	   &execute::eql,
	   &execute::pushb,
	   &execute::jfalse

   };

	while (t != op_eof){
		//std::cout << code_tk_string[t] << endl;
		(e.*exe[t])();
		t = *(code_tk*)(e.code + e.ip);
	}

	return 0;
}
