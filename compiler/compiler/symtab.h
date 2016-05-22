#pragma once
#include <map>
#include <string>
#include <iostream>

using namespace std;
class symtab
{
public:
	symtab(int);
	~symtab();

	void insert_id(string, char c);
	void insert_addr(string, int, char);
	void insert_array(string, int, char);

	bool check_symtab(string id);
	char type(string id);
	void print();

	int get_address(string id);
	void extend(symtab);

public:
	int pos;
	char* symarray;
	map<string, int> address;
	int MAX;
};

