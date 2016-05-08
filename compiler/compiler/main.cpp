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
	

	return 0;
}
