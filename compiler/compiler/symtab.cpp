#include "symtab.h"

symtab::symtab(int size)
{
	MAX = size;
	pos = 0;
	symarray = (char*)malloc(sizeof(char)*MAX);
	for (int i = 0; i<MAX; ++i) *(symarray + i) = 0;
}

bool
symtab::check_symtab(string id){
	std::map<string, int>::iterator it;
	it = address.find(id);
	if (it != address.end())
		return true;
	else
		return false;
}
void
symtab::extend(symtab b){
	this->pos = b.pos;
	free(symarray);
	symarray = (char*)malloc(sizeof(char)*pos*2);
	for (int i = 0; i < pos; ++i){
		*(symarray + i) = *(b.symarray + i);
	}
	address = b.address;
}

// just add id to map dont add to array

void symtab::insert_id(string id, char c){
	address[id] = pos;
	switch (c){
	case 'I':
		*(symarray + pos) = 'I';
		pos++;
		pos += sizeof(int);
		break;
	case 'C':
		*(symarray + pos) = 'C';
		pos++;
		pos += sizeof(char);
		break;
	case 'F':
		*(symarray + pos) = 'F';
		pos++;
		pos += sizeof(float);
		break;
	case 'S':
		*(symarray + pos) = 'S';
		pos++;
		pos += sizeof(string*);
		break;
	case 'B':
		*(symarray + pos) = 'B';
		pos++;
		pos += sizeof(char);
		break;
	}
}

void symtab::insert(string id, int val){
	// need to be already declared
	// taken care of that in parser
	*(symarray + pos) = 'I';
	pos += sizeof(char);
	*((int*)symarray + pos)=val;
}
void symtab::insert(string id, char val){
	*(symarray + pos) = 'C';
	 pos += sizeof(char);
	*(symarray + pos) = val;
}

void symtab::insert(string id, float val){
	*(symarray + pos) = 'F';
	pos += sizeof(float);
	*((float*)symarray + pos) = val;
}

void symtab::insert(string id, string val){
	// need to be already declared
	// taken care of that in parser
	*(symarray + pos) = 'S';
	pos += sizeof(char);
	*((string**)symarray + pos) = &val;
}

void symtab::insert(string id, bool val){
	// need to be already declared
	// taken care of that in parser
	*(symarray + pos) = 'B';
	pos += sizeof(char);
	*(symarray + pos) = val;
	// minimum is char, char > bool
}

char symtab::type(string id){
	int i = address[id];
	char x;
	try{
		x= *(symarray + i);
	}
	catch (exception e){
		std::cout << id << "  not found while getting type" << endl;
	}
	return x;
}

int symtab::get_address(string id){
	return address[id];
}

void symtab::print(){
	for (auto it = address.cbegin(); it != address.cend(); ++it)
	{
		std::cout << it->first << " address is: " << it->second << endl;
	}
}
symtab::~symtab()
{
	free(symarray);
}
