#include "parser.h"

parser::parser(token** c, int token_size)
{
	stack = new symtab(token_size * 10);
	this->token_list = c;
	// just have code be longer than token list
	code = (char*)malloc(sizeof(char*)*token_size*10);
	currtoken = 0;

}

/*this puts code enum to the list*/
void
parser::gen_op_code(code_tk t){

	*(code_tk*)(code +ip)= t;
	ip += sizeof(code_tk);

	cout <<  code_tk_string[t]   <<endl;

}

/*this adds value to the */
void
parser::gen_address(int addr){
	*(int*)(code + ip) = addr;
	ip += sizeof(int);
	cout <<addr << endl;

}

void
parser::gen_char(char t){
	*(code+ip) = t;
	ip += sizeof(char);
	cout << t << endl;
}

// this starts program
void 
parser::start_prog(){
	match(TK_BEGIN);
	var_decl();
	statements();
	match(TK_END);
	if (token_list[currtoken]->name == TK_EOF) {
		gen_op_code(op_eof);
		match(TK_EOF);
		return;
	}
	else error("No end of file");

}

// this declares item
void
parser::var_decl(){

	match(TK_BEGIN);

	decl();
	
	match(TK_END);


}

// this does inner declaration statements
void
parser::decl(){

	if (token_list[currtoken]->name == TK_END){
		// return if scope closed
		return;
	}
	else{
		// if scope continues
		char t = type();
		namelist(t);

	}
}

// this looks if declaration is multiple
void
parser::namelist(char t){
	// check current lists to see if we need tpo add
	string curr = token_list[currtoken]->id;

	// if it is in symtab  show erro
	if (stack->check_symtab(curr)) {
		error(" already declared " + curr);
		return;
	}

	//if not add to stack, just inserts to map declares type
	stack->insert_id(curr, t);
	match(TK_ID);
	// if there is comm skip
	if (token_list[currtoken]->name == TK_COMMA){
		match(TK_COMMA);
		namelist(t);
	}
	else if (token_list[currtoken]->name == TK_SEMICOLON){
		match(TK_SEMICOLON);
		decl();
	}
	else {
		error("no id");
		return;
	}
}

//this gives typeof variable
char
parser::type(){
	switch (token_list [currtoken]->name){
	case TK_CHAR_DEF:
		match(TK_CHAR_DEF);
		return 'C';
	case TK_INT_DEF:
		match(TK_INT_DEF);
		return 'I';
	case TK_BOOL_DEF:
		match(TK_BOOL_DEF);
		return 'B';
	case TK_STRING_DEF:
		match(TK_STRING_DEF);
		return 'S';
	case TK_FLOAT_DEF:
		match(TK_FLOAT_DEF);
		return 'F';
	default:
		error(" no type found");
		return 't';
	}
	currtoken++;
}

// this is statements
void
parser::statements(){
	//cout << " got into statements\n";

	// start scope
	match(TK_BEGIN);
	statment_types();
	match(TK_END);

}

// this checks for different type of statements
void
parser::statment_types(){
	//cout << " got into statement types\n";
	token_name curr = token_list[currtoken]->name;
	token_name curr2 = token_list[currtoken + 1]->name;

	switch (curr){
		// check all stuff that starts with tk_id
	case TK_ID:
		switch (curr2){
			// check if it is assignment of goto or proc
		case TK_EQUAL:
			assignment();
			break;
		case TK_OPEN:
			break;
		}
		break;

	// now check print token
	case TK_PRINT:
		this->print();
		break;
	// now if no more statement
	case TK_END:
		return;
	default:
		error(" bad statwement");
	}

}

// this assigns value to a variable
void
parser::assignment(){
	// get stuff to assign
	//cout << " got into assignment\n";
	int addr = stack->get_address(token_list[currtoken]->id);
	match(TK_ID);
	match(TK_EQUAL);
	
	//call id literrak, plus minus stuff
	expression();
	// now arthematic or procedure here

	match(TK_SEMICOLON);

	gen_op_code(op_pop);
	gen_address(addr);
	
	// now go check if there are more statements
	statment_types();
	// need to do calculator here

}

// these do calculator funct
void
parser::expression(){
	expression_mul_div();
	add_sub();
}
// addition, subtraction
void
parser::add_sub(){
	token_name curr = token_list[currtoken]->name;
	if (curr == TK_PLUS){
		match(TK_PLUS);
		expression_mul_div();
		gen_op_code(op_add);
		add_sub();
	}
	else if (curr == TK_MINUS){
		match(TK_MINUS);
		expression_mul_div();
		gen_op_code(op_sub);
		add_sub();
	}
}

// multiplication start
void
parser::expression_mul_div(){
	value();
	mul_div();
}

// multiplication, division end
void
parser::mul_div(){
	token_name curr = token_list[currtoken]->name;

	if (curr == TK_MUL){
		match(TK_MUL);
		value();
		gen_op_code(op_mul);
		mul_div();
	}
	else if (curr == TK_DIV){
		match(TK_DIV);
		value();
		gen_op_code(op_div);
		mul_div();
	}

}

// this checks if there are more expressions or address to be looked for
void
parser::value(){
	token_name curr = token_list[currtoken]->name;
	string id = token_list[currtoken]->id;
	// 1st look if it a value

	switch (curr){
	case TK_INT:
		gen_op_code(op_pushi);
		gen_address(token_list[currtoken]->int_value);
		match(TK_INT);

		break;
	case TK_ID:
		gen_op_code(op_push);
		gen_address(stack->get_address(id));
		match(TK_ID);
		break;

		// check if it is negation or positive
	case TK_PLUS:
		match(TK_PLUS);
		value();
		gen_op_code(op_pos);

		break;
	case TK_MINUS:
		match(TK_MINUS);
		value();
		gen_op_code(op_neg);
		break;
		// check for open close parenthesis
	case TK_OPEN:
		match(TK_OPEN);
		expression();
		match(TK_CLOSE);
	}
}


// print statement

void
parser::print(){
	match(TK_PRINT);
	match(TK_OPEN);
	// if there is string print string else print value
	if (token_list[currtoken]->name == TK_STRING){
		int i = token_list[currtoken]->id.length();
		char* cstr = new char[i];
		std::strcpy(cstr, token_list[currtoken]->id.c_str());

		for (int j = 0; j < i; ++j){
			gen_op_code(op_printc);
			gen_char(cstr[j]);
		}
		match(TK_STRING);

	}
	else 
		value();
		gen_op_code(op_print);	
	match(TK_CLOSE);
	match(TK_SEMICOLON);
	statment_types();
}

// this checks and increments current token position
void
parser::match(token_name t){
	if (t != token_list[currtoken]->name){
		std::cout << " wrong token " << token_name_string[token_list[currtoken]->name] << " instead of  " << token_name_string[t] << endl;
		exit(0);
	}
	else{
		//cout << "matched ";
		//token_list[currtoken]->print();
		currtoken++;
	}
}

// just error
void
parser::error(string e){
	cout << "Error: " << e << endl;
}

// destructor
parser::~parser()
{
	delete[] token_list;
	delete[] code;
}

// print code
void 
parser::code_print(){
	code_tk t;
	ip = 0;

	while (*(code_tk*)(code + ip) != op_eof){
		t = *(code_tk*)(code + ip);
		ip += sizeof(code_tk);

		if (t == op_pushi ||
			t == op_pop ||
			t == op_push 
			){
			int addr = *(int*)(code + ip);
			cout << code_tk_string[t] << "\t\t" << addr << endl;
			ip += sizeof(int);
			continue;
		}
		if (t == op_printc){
			char addr = *(char*)(code + ip);
			cout << code_tk_string[t] << "\t\t" << addr << endl;
			ip += sizeof(char);
			continue;

		}
		cout << code_tk_string[t] << endl;
	}
	cout << code_tk_string[*(code_tk*)(code + ip) ] << endl;

}