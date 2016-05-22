#include "parser.h"

// constructor that just copys pointer to token list, initializes code array, and symtab
parser::parser(token** c, int token_size)
{
	stack = new symtab(token_size * 10);
	this->token_list = c;
	// just have code be longer than token list
	code = (char*)malloc(sizeof(char*)*token_size*10);
	currtoken = 0;
}

/********************************************************************************/
/***************code addition to code list**********************************************/
/********************************************************************************/
/*this puts code enum to the list*/
void
parser::gen_op_code(code_tk t){

	*(code_tk*)(code +ip)= t;
	ip += sizeof(code_tk);

	//cout <<  code_tk_string[t]   <<endl;
}

/*this adds integer to the code array*/
void
parser::gen_address(int addr){
	*(int*)(code + ip) = addr;
	ip += sizeof(int);
	//cout <<addr << endl;

}

/*this adds char to the code array*/
void
parser::gen_char(char t){
	*(code+ip) = t;
	ip += sizeof(char);
	//cout << t << endl;
}

/*this adds bool to the code array*/
void
parser::gen_bool(bool v){
	*(bool*)(code + ip) = v;
	// since sometimes char is bigger than char
	ip += sizeof(char);
	//cout <<addr << endl;

}

/*this adds float to the code array*/
void
parser::gen_float(float t, int e){
	// e is exponential value that is added to float
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

/********************************************************************************/
/***************parser that creates code**********************************************/
/********************************************************************************/
// this starts program 
// body is 
/*  {
	{
	var declaration
	}
	{
	statements
	}
	proicedures
	}
*/

void 
parser::start_prog(){
	match(TK_BEGIN);
	var_decl(); 
	statements();
	gen_op_code(op_jmp); // save to skip procedure
	int hole=ip; // save for later
	gen_address(0);
	procedures();

	// skip procedures
	int save = ip;
	ip = hole;
	gen_address(save);
	ip = save;

	match(TK_END);
	if (token_list[currtoken]->name == TK_EOF) {
		gen_op_code(op_eof);
		match(TK_EOF);
		return;
	}
	else error("No end of file",' ',' ');

}

/********************************************************************************/
/***************parser declaration**********************************************/
/********************************************************************************/
// this starts declaration block that does multiple declaration
void
parser::var_decl(){

	match(TK_BEGIN);

	decl();
	
	match(TK_END);


}

// this calculates single declaration line
void
parser::decl(){

	if (token_list[currtoken]->name == TK_END){
		// return if scope closed
		return;
	}
	else{
		// if scope continues
		char t = type();
		if (token_list[currtoken]->name == TK_SQUARE_OPEN)
			array_decl(t);
		else
		namelist(t); // get names of ids declared and assign them type t and add to symtab
	}
}
// this is to declare arrays
void
parser::array_decl(char t){
	// check current lists to see if we need tpo add
	match(TK_SQUARE_OPEN);
	int size = token_list[currtoken]->int_value;
	match(TK_INT);
	match(TK_SQUARE_CLOSE);

	string curr = token_list[currtoken]->id;

	// if it is in symtab  show erro
	if (stack->check_symtab(curr)) {
		error(" already declared ", curr, ' ');
		return;
	}

	//if not add to stack, just inserts to map declares type
	stack->insert_array(curr, size, t);
	match(TK_ID);

	if (token_list[currtoken]->name == TK_SEMICOLON){
		match(TK_SEMICOLON);
		decl();
	}
	else {
		error("no id, there is ", token_name_string[token_list[currtoken]->name], ' ');
		std::exit(0);
		return;
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
		std::exit(0);
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
		std::exit(0);
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
	case TK_PROCEDURE_DEF:
		match(TK_PROCEDURE_DEF);
		return 'P';

	// insert integer value to symarray

	// now for literals dont match 
	case TK_CHAR:
		return 'C';
	case TK_INT:
		return 'I';
	case TK_BOOL:
		return 'B';
	case TK_STRING:
		return 'S';
	case TK_FLOAT:
		return 'F';
	// for id look at symbol table
	case TK_ID:
		return stack->type(token_list[currtoken]->id);
	default:
		error(" no type found"," ", " ");
		return 't';
	}
	currtoken++;
}

/********************************************************************************/
/***************parser statements**********************************************/
/********************************************************************************/

//procedures

void 
parser::procedures(){
	if (token_list[currtoken]->name == TK_END){
		return; // if thre is no more procedure just return
	}
	else {
		string name = token_list[currtoken]->id;
		char t = type();
		match(TK_ID);
		stack->insert_addr(name, ip, t); // insert current code address to the symtab
		statements(); // just run statements
		gen_op_code(op_restore); // go back to place after call
		procedures();
	}
}

// this is for calling procedure
void
parser::procedure_call(){
	string name = token_list[currtoken]->id;
	int proc_addr = stack->get_address(name);

	match(TK_ID);
	match(TK_OPEN);
	match(TK_CLOSE);
	match(TK_SEMICOLON);

	// save address to stack
	gen_op_code(op_pushi); // go to procedure
	int ret_place = ip;
	gen_address(0);  // save position to return

	gen_op_code(op_push);

	gen_address(proc_addr);
	gen_op_code(op_restore); // this uses pushed value to go to ip

	int save = ip;
	ip = ret_place;
	gen_address(save);
	ip = save;


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
			procedure_call();
			break;
		case TK_SQUARE_OPEN:
			array_assign();
			break;
		default:
			error("syntax error ", token_name_string[curr2], " not correct");
			std::exit(0);
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
	case TK_WHILE:
		m_while();
		break;
	case TK_IF:
		m_if();
		break;
	case TK_SWITCH:
		m_switch();
		break;
	case TK_FOR:
		m_for();
		break;
	case TK_END:
		return;
		// for switch case statements
	case TK_CASE:
		return;
	case TK_DEFAULT:
		return;

	default:
		error(" bad statement", " " , " ");
		std::exit(0);
	}
	// now go check if there are more statements
	statment_types();
	// need to do calculator here
}

/********************************************************************************/
/***************parser assignment**********************************************/
/********************************************************************************/

void
parser::array_assign(){
	string id = token_list[currtoken]->id;
	int addr = stack->get_address(id);
	match(TK_ID);
	match(TK_SQUARE_OPEN);
	int i = token_list[currtoken]->int_value;
	match(TK_INT);
	char type = *(stack->symarray + addr + sizeof(char)+sizeof(int));

	code_tk get_code = op_placeholder;

	int elem_size = 0;
	switch (type){
	case 'I':
		elem_size = sizeof(int);
		get_code = op_puti;
		break;
	case 'C':
		elem_size = sizeof(char);
		get_code = op_putc;
		break;
	case 'B':
		elem_size = sizeof(bool);
		get_code = op_putb;
		break;
	case 'F':
		elem_size = sizeof(float);
		get_code = op_putf;
		break;

	default:
		error(id, " got bad array type during declaration for ", type);
		std::exit(0);
	}


	int lo = *(int*)(stack->symarray + addr + sizeof(int) + 2 * sizeof(char)); // array type, type,lo,Hi,.......
	int hi = *(int*)(stack->symarray + addr + sizeof(int)+ 2 * sizeof(char)+sizeof(int)); // array type, type,lo,Hi,.......
	
	addr = *(int*)(stack->symarray + addr + sizeof(char));

	if (i < lo || i > hi){
		error("Out of index array", " ", id);
		std::exit(0);
	}
	match(TK_SQUARE_CLOSE);

	// now generate code
	gen_op_code(op_pushi);
	gen_address(i);
	gen_op_code(op_pushi);
	gen_address(lo);

	gen_op_code(op_sub);

	gen_op_code(op_pushi);
	gen_address(elem_size);

	gen_op_code(op_mul);


	gen_op_code(op_pushi);
	gen_address(addr);

	gen_op_code(op_add);

	
	match(TK_EQUAL);
	
	//call id literrak, plus minus stuff
	expression();
	// now arthematic or procedure here

	match(TK_SEMICOLON);

	gen_op_code(get_code);

}


// this assigns value to a variable
void
parser::assignment(){
	// get stuff to assign
	//cout << " got into assignment\n";
	if (token_list[currtoken+1]->name == TK_SQUARE_OPEN){
		array_assign();
		return;
	}
	int addr = stack->get_address(token_list[currtoken]->id);

	match(TK_ID);
	
	match(TK_EQUAL);
	
	//call id literrak, plus minus stuff
	expression();
	// now arthematic or procedure here

	match(TK_SEMICOLON);

	gen_op_code(op_pop);
	gen_address(addr);
	

}

/********************************************************************************/
/***************parser exression**********************************************/
/********************************************************************************/
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

void
parser::array_value(){
	string id = token_list[currtoken]->id;
	int addr = stack->get_address(id);
	match(TK_ID);
	match(TK_SQUARE_OPEN);
	int i = token_list[currtoken]->int_value;
	match(TK_INT);
	char type = *(stack->symarray + addr + sizeof(char)+sizeof(int));

	code_tk get_code = op_placeholder;

	int elem_size = 0;
	switch (type){
	case 'I':
		elem_size = sizeof(int);
		get_code = op_geti;
		break;
	case 'C':
		elem_size = sizeof(char);
		get_code = op_getc;
		break;
	case 'B':
		elem_size = sizeof(bool);
		get_code = op_getb;
		break;
	case 'F':
		elem_size = sizeof(float);
		get_code = op_getf;
		break;

	default:
		error(id, " got bad array type during declaration for ", type);
		std::exit(0);
	}


	int lo = *(int*)(stack->symarray + addr + 2 * sizeof(char) + sizeof(int)); // array type, type,lo,Hi,.......
	int hi = *(int*)(stack->symarray + addr + 2 * sizeof(char)+ 2* sizeof(int)); // array type, type,lo,Hi,.......
	addr = *(int*)(stack->symarray + addr + sizeof(char));

	if (i < lo || i > hi){
		error("Out of index array", " ", id);
		std::exit(0);
	}
	match(TK_SQUARE_CLOSE);

	// now generate code
	gen_op_code(op_pushi);
	gen_address(i);
	gen_op_code(op_pushi);
	gen_address(lo);

	gen_op_code(op_sub);

	gen_op_code(op_pushi);
	gen_address(elem_size);

	gen_op_code(op_mul);


	gen_op_code(op_pushi);
	gen_address(addr);

	gen_op_code(op_add);

	gen_op_code(get_code);

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
		if (token_list[currtoken + 1]->name == TK_SQUARE_OPEN)
			array_value();
		else{
			gen_op_code(op_push);
			gen_address(stack->get_address(id));
			match(TK_ID);
		}
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
					std::exit(0);

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
}

/********************************************************************************/
/***************parser loops and conditions**********************************************/
/********************************************************************************/
// do while loop
void parser::do_while(){
	match(TK_DO);
	int target = ip;
	statements();
	match(TK_WHILE);

	expression();
	gen_op_code(op_jtrue);
	gen_address(target);

	match(TK_SEMICOLON);
	// now go check if there are more statements
}

//while
void parser::m_while(){
	match(TK_WHILE);
	
	// for jump 
	int cond = ip;
	expression();
	gen_op_code(op_jfalse);

	// save place for jfalse addr that is not compolete yet
	int hole = ip;
	gen_address(10);

	statements();

	gen_op_code(op_jmp);
	gen_address(cond);

	// fill hole of jfalse
	int save = ip;
	ip = hole;
	gen_address(save);
	ip = save;

}

// if statement
void parser::m_if(){
	match(TK_IF);

	// for jump 
	expression();
	gen_op_code(op_jfalse);

	// save place for jfalse addr that is not compolete yet
	int hole_if = ip;
	gen_address(10);
	statements();
	int save;
	token_name curr = token_list[currtoken]->name;
	if (curr == TK_ELSE){
		// create jump for if if is true
		gen_op_code(op_jmp);
		int hole = ip;
		gen_address(0);

		// now put jfalse address here
		save = ip;
		ip = hole_if;
		gen_address(save);
		ip = save;
		
		//set the hole to pe parched op_jmp
		hole_if = hole;

		// now do else
		match(TK_ELSE);
		statements();

	}
	// fill hole of jfalse
	save = ip;
	ip = hole_if;
	gen_address(save);
	ip = save;
	
}
/*************************************************/
// while statement
void parser::m_switch(){
	match(TK_SWITCH);
	// this looks for expression and type

	value();
	match(TK_BEGIN);
	gen_op_code(op_jmp);
	int first = ip;
	gen_address(0);
	// create 2 jmp, 1 jumps over send jum. 2nd jmp jumps over entire switch case,
	//then if a case is true  i jump over to 2nd jmp
	int skip = ip;
	gen_op_code(op_jmp);
	int second = ip;
	gen_address(0);

	int temp = ip;
	ip = first;
	gen_address(temp);
	ip = temp;

	token_name curr = token_list[currtoken]->name;
	while (curr == TK_CASE){
		match(TK_CASE);
		// duplicate main value
		gen_op_code(op_dup);
		value();
		match(TK_COLON);
		// compate values
		gen_op_code(op_eql);
		// jump if false
		gen_op_code(op_jfalse);
		int hole = ip;
		gen_address(0);

		statment_types();
		// since statement is true skip
		gen_op_code(op_jmp);
		gen_address(skip);

		int s = ip;
		ip = hole;
		gen_address(s);
		ip = s;
		curr = token_list[currtoken]->name;

	}
	if (curr == TK_DEFAULT){
		match(TK_DEFAULT);
		match(TK_COLON);
		statment_types();
	}
	int save = ip;
	ip = second;
	gen_address(save);
	ip = save;

	match(TK_END);
	gen_op_code(op_remove);
}

// this checks and increments current token position
void parser::m_for(){
	match(TK_FOR);
	match(TK_OPEN);

	// do assignment
	assignment();
	int comp_hole = ip;  // need to jump here later

	// do expression
	expression();
	match(TK_SEMICOLON);
	gen_op_code(op_jfalse); // when false escape for
	// save hole to jump if true
	int when_wrong = ip;
	gen_address(0);

	gen_op_code(op_jmp);
	// jump to statement when true;
	int when_right = ip;
	gen_address(0);

	int increment = ip;

	// do increment stuff
	// get stuff to assign
	//cout << " got into assignment\n";
	int addr = stack->get_address(token_list[currtoken]->id);
	match(TK_ID);
	match(TK_EQUAL);

	//call id literrak, plus minus stuff
	expression();
	// now arthematic or procedure here

	match(TK_CLOSE);
	gen_op_code(op_pop);
	gen_address(addr);

	// go to compare after assignment
	gen_op_code(op_jmp);
	gen_address(comp_hole);

	// now go to statement afeer compare 
	// for that fill jump
	int save = ip;
	ip = when_right;
	gen_address(save);
	ip = save;
	// now go to statement
	statements();

	// now jump to increment
	gen_op_code(op_jmp);
	gen_address(increment);

	// now fill hole for when comparision is wrong
	save = ip;
	ip = when_wrong;
	gen_address(save); // when wrong skip for loop
	ip = save;

}

void
parser::match(token_name t){
	if (t != token_list[currtoken]->name){
		error(" wrong token ", token_name_string[token_list[currtoken]->name], " ");
		error("instead of ",token_name_string[t], " ");
		std::exit(0);
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
		std::cout << "address:  " << ip << "\t";
		t = *(code_tk*)(code + ip);
		ip += sizeof(code_tk);

		if (t == op_pushi ||
			t == op_pop ||
			t == op_push ||
			t == op_jfalse ||
			t == op_jmp||
			t ==op_jtrue
			){
			int addr = *(int*)(code + ip);
			std::cout << code_tk_string[t]<< std::endl;
			std::cout << "address:  " << ip << " " << addr << std::endl;
			ip += sizeof(int);
			continue;
		}
		if (t == op_printc || t == op_pushc){
			char addr = *(code + ip);
			std::cout << code_tk_string[t] << std::endl;
			std::cout << "address:  " << ip << "\t\t";
			std::cout<<addr<<std::endl;
			ip += sizeof(char);
			continue;

		}
		if (t == op_pushf){
			float addr = *(float*)(code + ip);
			std::cout << code_tk_string[t] << std::endl;
			std::cout << "address:  " << ip << "\t\t" << addr << endl;
			ip += sizeof(float);
			continue;
		}
		if (t == op_pushb){
			bool addr = *(bool*)(code + ip);
			std::cout << code_tk_string[t] << std::endl;
			std::cout << "address:  " << ip << "\t\t";
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