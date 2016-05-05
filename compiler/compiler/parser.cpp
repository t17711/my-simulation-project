#include "parser.h"

parser::parser(token** c, int token_size)
{
	stack = new symtab(token_size * 10);
	this->token_list = c;
	// just have code be longer than token list
	code = (char*)malloc(sizeof(char*)*token_size*10);
	currtoken = 0;

}

void parser::gen_op_code(code_tk t){

	*(code_tk*)(code +ip)= t;
	ip += sizeof(code_tk);

	cout <<  code_tk_string[t]   <<endl;

}

void parser::gen_op_code_var(int addr){
	*(int*)(code + ip) = addr;
	ip += sizeof(int);
	cout << "inserted  address:  " <<addr << endl;

}

void parser::gen_address(int t){
	*(int*)code = t;
	ip += sizeof(int);
	cout << "inserted  \t: token num  " << t << endl;
}

// this starts program
void 
parser::start_prog(){
	match(TK_BEGIN);
	var_decl();
	statements();
	match(TK_END);
	if (token_list[currtoken]->name == TK_EOF) {
		gen_op_code(op_end);
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

void parser::namelist(char t){
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
char parser::type(){
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

void parser::statements(){
	//cout << " got into statements\n";

	// start scope
	match(TK_BEGIN);
	statment_types();
	match(TK_END);

}

void parser::statment_types(){
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
	case TK_FOR:

		break;
	// now if no more statement
	case TK_END:
		return;
	default:
		error(" bad statwement");
	}

}

void parser::assignment(){
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
	gen_op_code_var(addr);
	
	// now go check if there are more statements
	statment_types();
	// need to do calculator here

}

void parser::expression(){
	expression_mul_div();
	add_sub();
}

void parser::add_sub(){
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

void parser::expression_mul_div(){
	value();
	mul_div();
}

void parser::mul_div(){
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

void parser::value(){
	token_name curr = token_list[currtoken]->name;
	string id = token_list[currtoken]->id;
	// 1st look if it a value

	switch (curr){
	case TK_INT:
		gen_op_code(op_pushi);
		gen_op_code_var(token_list[currtoken]->int_value);
		match(TK_INT);

		break;
	case TK_ID:
		gen_op_code(op_push);
		gen_op_code_var(stack->get_address(id));
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

void parser::match(token_name t){
	if (t != token_list[currtoken]->name)
		error(" wrong token");
	else{
		//cout << "matched ";
		//token_list[currtoken]->print();
		currtoken++;
	}
}

void parser::error(string e){
	cout << "Error: " << e << endl;
}

parser::~parser()
{
	delete[] token_list;
	delete[] code;
}