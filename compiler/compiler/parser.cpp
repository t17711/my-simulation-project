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

	//cout <<  code_tk_string[t]   <<endl;
}

/*this adds value to the */
void
parser::gen_address(int addr){
	*(int*)(code + ip) = addr;
	ip += sizeof(int);
	//cout <<addr << endl;

}

void
parser::gen_char(char t){
	*(code+ip) = t;
	ip += sizeof(char);
	//cout << t << endl;
}

void
parser::gen_bool(bool v){
	*(bool*)(code + ip) = v;
	// since sometimes char is bigger than char
	ip += sizeof(char);
	//cout <<addr << endl;

}

void
parser::gen_float(float t, int e){
	float x = t;
	if (e > 0){
		while (e > 0){
			x = x * 10;
			e--;
		}
	}
	else if (e < 0){
		while (e < 0){
			x = x / 10;
			e++;
		}
	}
	*(float*)(code+ip) = x;
	ip += sizeof(float);
	//cout << t << endl;
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
	else error("No end of file",' ',' ');

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
		error(" already declared ",curr,' ');
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
		error("no id there is ", token_name_string[token_list[currtoken]->name], ' ');
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
		error(" no type found"," ", " ");
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
		}
		break;

	// now check print token
	case TK_PRINT:
		this->print();
		break;
	// now if no more statement
	case TK_DO:
		do_while();
		break;
	case TK_END:
		return;
	default:
		error(" bad statement", " " , " ");
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
	switch (curr){
	case TK_PLUS:
		match(TK_PLUS);
		expression_mul_div();
		gen_op_code(op_add);
		add_sub();
		break;
	case TK_MINUS:
		match(TK_MINUS);
		expression_mul_div();
		gen_op_code(op_sub);
		add_sub();
		break;
	case TK_OR:
		match(TK_OR);
		expression_mul_div();
		gen_op_code(op_or);
		add_sub();
		break;
	default:
		// so empty
		break;
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

	switch (curr){
	case TK_AND:
		match(TK_AND);
		value();
		gen_op_code(op_and);
		expression_mul_div();
		break;
	case TK_MUL:
		match(TK_MUL);
		value();
		gen_op_code(op_mul);
		expression_mul_div();
		break;

	case TK_DIV:
		match(TK_DIV);
		value();
		gen_op_code(op_div);
		expression_mul_div();
		break;

	case TK_GREATER:
		match(TK_GREATER);
		expression_mul_div();
		gen_op_code(op_greater);
		expression_mul_div();
		break;

	case TK_GREATER_EQUAL:
		match(TK_GREATER_EQUAL);
		value();
		gen_op_code(op_greater_eql);
		expression_mul_div();
		break;

	case TK_LESS:
		match(TK_LESS);
		value();
		gen_op_code(op_less);
		expression_mul_div();
		break;

	case TK_LESS_EQUAL:
		match(TK_LESS_EQUAL);
		value();
		gen_op_code(op_less_eql);
		expression_mul_div();
		break;

	case TK_EQUAL_COMP:
		match(TK_EQUAL_COMP);
		value();
		gen_op_code(op_eql);
		expression_mul_div();
		break;
	default:
		// so empty
		break;
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

	// check for char value
	case TK_CHAR:
		gen_op_code(op_pushc);
		gen_char(token_list[currtoken]->char_val);
		match(TK_CHAR);
		break;

	// float value
	case TK_FLOAT:
		gen_op_code(op_pushf);
		gen_float(token_list[currtoken]->float_value, token_list[currtoken]->exp);
		match(TK_FLOAT);
		break;
	
	// true false
	case TK_TRUE:
		gen_op_code(op_pushb);
		gen_bool(true);
		match(TK_TRUE);
		break;

	case TK_FALSE:
		gen_op_code(op_pushb);
		gen_bool(false);
		match(TK_FALSE);
		break;

	case TK_NOT:
		match(TK_NOT);
		value();
		gen_op_code(op_not);
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

	default:
		break;
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
			if (cstr[j] == '\\'){
				j++;
				switch (cstr[j]){
				case 'n':
					gen_char('\n');
					break;
				case 't':
					gen_char('\t');
					break;
				default:
					error("bad string operator ", cstr[j], " ");
					exit(0);

				}

				continue;
			}
			gen_char(cstr[j]);
		}
		match(TK_STRING);

	}
	else if (token_list[currtoken]->name == TK_CHAR){
		gen_op_code(op_printc);
		gen_char(token_list[currtoken]->char_val);
		match(TK_CHAR);
	}
	else {
		value();
		gen_op_code(op_print);
	}
	match(TK_CLOSE);
	match(TK_SEMICOLON);
	statment_types();
}

// do while loop
void parser::do_while(){
	match(TK_DO);
	int target = ip;
	statements();
	match(TK_WHILE);
	conditions(target);
	match(TK_SEMICOLON);

	// now go check if there are more statements
	statment_types();
	// need to do calculator here

}

void parser::conditions(int target){
	expression();
	gen_op_code(op_jfalse);
	gen_address(target);
}

// this checks and increments current token position
void
parser::match(token_name t){
	if (t != token_list[currtoken]->name){
		error(" wrong token ", token_name_string[token_list[currtoken]->name], " ");
		error("instead of ",token_name_string[t], " ");
		exit(0);
	}
	else{
		//cout << "matched ";
		//token_list[currtoken]->print();
		currtoken++;
	}
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
			t == op_push ||
			t == op_jfalse
			){
			int addr = *(int*)(code + ip);
			std::cout << code_tk_string[t] << "\t\t" << addr << std::endl;
			ip += sizeof(int);
			continue;
		}
		if (t == op_printc || t == op_pushc){
			char addr = *(char*)(code + ip);
			std::cout << code_tk_string[t] << "\t\t" << addr << std::endl;
			ip += sizeof(char);
			continue;

		}
		if (t == op_pushf){
			float addr = *(float*)(code + ip);
			std::cout << code_tk_string[t] << "\t\t";
			printf("%f\n",addr);
			ip += sizeof(float);
			continue;
		}
		if (t == op_pushb){
			bool addr = *(bool*)(code + ip);
			std::cout << code_tk_string[t] << "\t\t";
			if (addr)
				printf("true \n");
			else
				printf("flase \n");

			ip += sizeof(char);
			continue;
		}
		std::cout << code_tk_string[t] << std::endl;
	}
	std::cout << code_tk_string[*(code_tk*)(code + ip) ] << std::endl;
	std::cout << "final pos  " << ip<<std::endl;

}