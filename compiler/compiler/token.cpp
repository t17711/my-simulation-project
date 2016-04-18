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

token::token(const token_name &name, const std::string &id)
{
	this->name = name;
	this->id =id;
	this->int_value = 0;
	this->float_value = 0;;
	this->exp = 0;


}
token::token(const token_name & name, const int & int_value)
{
	this->name = name;
	this->int_value = int_value;
	this->id = "";
	this->float_value = 0;
	this->exp = 0;


}

token::token(const token_name & name, const int & int_val, const int & float_value)
{
	this->name = name;
	this->float_value = float_value;
	this->id = "";
	this->int_value = int_val;
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
token::~token()
{
}
void 
token::print(){
	std::cout << get_token_name(name) << "\t";
	switch (name){
		case TK_ID:
			std::cout << id << "\t";
		case TK_INT:
			std::cout << int_value << "\t";
			break;
		case TK_FLOAT:
			std::cout << float_value << "\t";
			break;
		default:
				break;

	}
	std::cout << std::endl;
}
std::string
token::get_token_name(token_name tk){
	switch (tk){
	case	TK_ID: // identifier
		return	"TK_ID";
	case	TK_BEGIN: // { open scope
		return 	"TK_BEGIN";
	case	TK_END: // } end scope
		return 	"TK_END";
	case	TK_INT: // INTEGER
		return	"TK_INT";

	case	TK_FLOAT: // INTEGER
		return	"TK_FLOAT";

	case	TK_STRING: // string
		return 	"TK_STRING";

	case	TK_BOOL: // string
		return 	"TK_BOOL";

	case	TK_CHAR: // char
		return 	"TK_CHAR";
	case	TK_PLUS:
		return 	"TK_PLUS";

	case	TK_MINUS:
		return 	"TK_MINUS";

	case	TK_MUL:
		return 	"TK_MUL";

	case	TK_DIV:
		return 	"TK_DIV";

	case	TK_OPEN:
		return 	"TK_OPEN";

	case	TK_CLOSE:
		return 	"TK_CLOSE";

	case	TK_GREATER:
		return 	"TK_GREATER";

	case	TK_LESS:
		return 	"TK_LESS";

	case	TK_EQUAL:
		return 	"TK_EQUAL";

	case	TK_QUES:
		return 	"TK_QUES";

	case	TK_GREATER_EQUAL:
		return 	"TK_GREATER_EQUAL";

	case	TK_LESS_EQUAL:
		return 	"TK_LESS_EQUAL";

	case	TK_SEMICOLON:
		return 	"TK_SEMICOLON";

	case	TK_COLON:
		return 	"TK_COLON";

	case	TK_INCLUDE:
		return 	"TK_INCLUDE";

	case	TK_DEFINE:
		return 	"TK_DEFINE";

	case	TK_COMMA:
		return 	"TK_COMMA";

	case	TK_DOT:
		return 	"TK_DOT";

	case	TK_EOF:
		return 	"TK_EOF";

	case	TK_FOR:
		return 	"TK_FOR";

	case	TK_WHILE:
		return 	"TK_WHILE";

	case	TK_INT_DEF:
		return 	"TK_INT_DEF";

	case	TK_FLOAT_DEF:
		return 	"TK_FLOAT_DEF";

	case	TK_BOOL_DEF: // string
		return 	"TK_BOOL_DEF";

	case	TK_STRING_DEF:
		return 	"TK_STRING_DEF";

	case	TK_RETURN:
		return 	"TK_RETURN";

	default:
		return 	"NOT_TOKEN";
	}
}