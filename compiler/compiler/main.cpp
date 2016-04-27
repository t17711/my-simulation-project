#pragma warning(disable:4996)

#include "scanner.cpp"
using namespace std;
int main(int argc, char **argv)
{
	scanner c = scanner();
	c.get_token();
	c.print();

	return 0;
}
