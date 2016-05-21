#include "token.h"
token::token(){
	this->name = TK;
	this->id = "";
	this->int_value = 0;
	this->float_value = 0;
	this->exp = 0;


}

token::token(const token_name & name)
{
	this->name = name;
	this->id = "";
	this->int_value = 0;
	this->float_value = 0;
	this->exp = 0;

}

token& 
token::operator = (const token& t){
	id +=t.id;
	name = t.name;
	int_value = t.int_value;
	float_value = t.float_value;
	exp = t.exp;
	return *this;

}

void 
token::print(){
	std::cout << token_name_string[name] << "\t";
	switch (name){
		case TK_ID:
			std::cout << id << "\t";
			break;
		case TK_INT:
			std::cout << int_value << "\t";
			break;
		case TK_FLOAT:
			std::cout << float_value << "\t";
			break;
		case TK_STRING:
			std::cout << id << "\t";
			break;
		default:
				break;

	}
	std::cout << std::endl;
}
