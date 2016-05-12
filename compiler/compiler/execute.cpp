#include "execute.h"


/// completed code //////////////////
execute::execute(char* code, symtab* tab){
	this->code = code;
	this->table = tab;
	ip = 0;
	is = 0;
	// max stack is max 100 char = 25 int
	stack = (char*)(malloc(sizeof(char)* 1000));

}

int execute::push(){
	ip += sizeof(code_tk);

	int addr = *(int*)(code + ip);
	ip += sizeof(int);

	char type = *(table->symarray + addr);
	switch (type){
	case 'I':
		*(int*)(stack + is) = *(int *)(table->symarray + addr + sizeof(char));
		is += sizeof(int);
		*(stack+is) = 'I';
		is += sizeof(char);
		break;
	case 'C':
		*(stack + is) = *(table->symarray + addr + sizeof(char));
		is += sizeof(char);
		*(stack + is) = 'C';
		is += sizeof(char);

		break;
	case 'F':
		*(float*)(stack + is) = *(float *)(table->symarray + addr + sizeof(char));
		is += sizeof(float);
		*(stack + is) = 'F';
		is += sizeof(char);

		break;
	case 'B':
		*(bool*)(stack + is) = *(bool *)(table->symarray + addr + sizeof(char));
		is += sizeof(char);
		*(stack + is) = 'B';
		is += sizeof(char);

		break;
	}
	return 1;
}

int execute::pushi(){
	ip += sizeof(code_tk);
	
	int val =*(int*)(code + ip);

	ip += sizeof(int);

	*(int*)(stack + is) = val;

	is += sizeof(int);

	*(stack + is) = 'I';

	is += sizeof(char);

	return 1;
}

int execute::pushb(){
	ip += sizeof(code_tk);

	bool val = *(bool*)(code + ip);

	ip += sizeof(char);

	*(bool*)(stack + is) = val;

	is += sizeof(char);

	*(stack + is) = 'B';

	is += sizeof(char);

	return 1;
}

int execute::pushc(){
	ip += sizeof(code_tk);

	char val = *(code + ip);

	ip += sizeof(char);

	*(stack + is) = val;

	is += sizeof(char);

	*(stack + is) = 'C';

	is += sizeof(char);

	return 1;
}

int execute::pushf(){
	ip += sizeof(code_tk);

	float val = *(float*)(code + ip);

	ip += sizeof(float);

	*(float*)(stack + is) = val;

	is += sizeof(float);

	*(stack + is) = 'C';

	is += sizeof(char);

	return 1;
}

int execute::pop(){
	ip += sizeof(code_tk);
	int addr = *(int*)(code + ip);
	ip += sizeof(int);

	// got to type
	is -= sizeof(char);
	char type1 = *(stack + is);

	char type2 = *(table->symarray + addr);
	addr += sizeof(char);

	switch (type2){
	case 'I':
		switch (type1){
		case 'I':
			is -= sizeof(int);
			*(int*)(table->symarray + addr) = *(int*)(stack + is);
			break;
		case 'F':
			is -= sizeof(float);
			*(int*)(table->symarray + addr) =(int) *(float*)(stack + is);
			break;
		case 'C':
			is -= sizeof(char);
			*(int*)(table->symarray + addr) = (int)*(stack + is);
			break;
		default:
			error("bad type ", type1, " ");
			exit(0);
		}
		
		break;
	case 'C':
		switch (type1){
		case 'I':
			is -= sizeof(int);
			*(table->symarray + addr) =(char) *(int*)(stack + is);
			break;
		case 'F':
			is -= sizeof(float);
			*(table->symarray + addr) = (char) (int)*(float*)(stack + is);
			break;
		case 'C':
			is -= sizeof(char);
			*(table->symarray + addr) = *(stack + is);
			break;
		default:
			error("bad type ", type1, " ");
			exit(0);
		}
		break;
	case 'F':
		switch (type1){
		case 'I':
			is -= sizeof(int);
			*(int*)(table->symarray + addr) = *(int*)(stack + is);
			break;
		case 'F':
			is -= sizeof(float);
			*(int*)(table->symarray + addr) = (int)*(float*)(stack + is);
			break;
		case 'C':
			is -= sizeof(char);
			*(int*)(table->symarray + addr) = (int)*(stack + is);
			break;
		default:
			error("bad type ", type1, " ");
			exit(0);
		}
		break;
	case 'B':
		if (type1 == 'B'){
			is -= sizeof(char);
			*(bool*)(table->symarray + addr) = *(bool*)(stack + is);
		}
		break;
	default:
		error("bad type ", type2, " ");
		exit(0);
	}
	return 1;
}
	
int execute::print(){
	ip += sizeof(code_tk);
	is -= sizeof(char);
	char type = stack[is];
	bool v = false;
	switch (type){
	case 'I':
		is -= sizeof(int);
		std::cout << *(int*)(stack + is);
		break;
	case 'C':
		is -= sizeof(char);
		std::cout << *(stack + is);

		break;
	case 'F':
		is -= sizeof(float);
		std::cout << *(float*)(stack + is);
		break;
	case 'B':
		is -= sizeof(char);
		 v = *(bool*)(stack + is);
		if (v)
			std::cout << "TRUE";
		else
			std::cout << "FALSE";
		break;
	default:
		error("bad type ", type, " ");
		exit(0);
	}
	return 1;
}

int execute::printc(){
	ip += sizeof(code_tk);
	std::cout << *(code + ip);
	ip += sizeof(char);
	return 1;
}

int execute::add(){
	ip += sizeof(code_tk);
	// go back to type
	is -= sizeof(char);
	char type1 = *(stack+is);

	int val1 = 0;
	char valc;
	float valf;
	char type2;
	switch (type1){
		case 'I':
			// go to int value and copy
			is -= sizeof(int);
			val1 = *(int*)(stack + is);

			// go to type of next val
			is -= sizeof(char);
			type2 = *(stack+is);

			// ad if they are char or 
			switch (type2){
			case 'I':
				// go  to int value
				is -= sizeof(int);

				// copy int vakue
				*(int*)(stack + is) = val1 + *(int*)(stack + is);
				// go ahead to type value
				is += sizeof(int);

				// update type
				*(stack + is) = 'I';
				is += sizeof(char);
				break;
			case 'C':
				// go back to char value
				is -= sizeof(char);

				// co[py int vakue
				*(stack + is) =val1 + *(stack + is);

				// go ahead to type value
				is += sizeof(char);

				// update type
				*(stack + is) = 'C';
				is += sizeof(char);

				break;
			case 'F':
					// go back to char value
					is -= sizeof(float);

					// co[py int vakue
					*(float*)(stack + is) = (float)val1 + *(float*)(stack + is);

					// go ahead to type value
					is += sizeof(float);

					// update type
					*(stack + is) = 'F';
					is += sizeof(char);

					break;
			default:
				error("type error", " ", " ");
				exit(0);
			}
			break;
		case 'C':
			// 1st val is char
			is -= sizeof(char);
			// copy char
			valc = *(stack + is);
			is -= sizeof(char);

			type2 = *(stack + is);
			// ad if they are char or 
			switch (type2){
			case 'I':
				// go  to int value
				is -= sizeof(int);

				// copy int vakue+char
				*(stack + is) =valc + *(int*)(stack + is);
				// go ahead to type value
				is += sizeof(char);

				// update type
				*(stack + is) = 'C';
				is += sizeof(char);
				break;
			case 'C':
				// go back to char value
				is -= sizeof(char);

				// co[py char vakue	 
				*(stack + is) = valc + *(stack + is);

				// go ahead to type value
				is += sizeof(char);

				// update type
				*(stack + is) = 'C';
				is += sizeof(char);
				break;
			case 'F':
				// go back to char value
				is -= sizeof(float);

				// co[py int vakue
				*(float*)(stack + is) = (float)valc + *(float*)(stack + is);

				// go ahead to type value
				is += sizeof(float);

				// update type
				*(stack + is) = 'F';
				is += sizeof(char);
				break;
			default:
				error("type error", " ", " ");
				exit(0);
			}
			break;
		case 'F':
			// 1st val is float
			is -= sizeof(float);
			// copy char
			valf = *(float*)(stack + is);

			is -= sizeof(char);
			type2 = *(stack+is);

			// ad if they are char or 
			switch (type2){
			case 'I':
				// go  to int value
				is -= sizeof(int);

				// copy int vakue+char
				*(float*)(stack + is) =valf+ *(int*)(stack + is);
				// go ahead to type value
				is += sizeof(float);

				// update type
				*(stack + is) = 'F';
				is += sizeof(char);
				break;
			case 'C':
				// go back to char value
				is -= sizeof(char);

				// co[py char vakue
				*(float*)(stack + is) = valf + *(stack + is);

				// go ahead to type value
				is += sizeof(float);

				// update type
				*(stack + is) = 'F';
				is += sizeof(char);
				break;
			case 'F':
				// go back to char value
				is -= sizeof(float);

				// co[py int vakue
				*(float*)(stack + is) = valf + *(float*)(stack + is);

				// go ahead to type value
				is += sizeof(float);

				// update type
				*(stack + is) = 'F';
				is += sizeof(char);
				break;
			default:
				error("type error", " ", " ");
				exit(0);
			}
			break;
		default:
			error("type error", " ", " ");
			exit(0);
		}
	
	return 1;
}

int execute::sub(){
	// negate last #
	neg();
	ip -= sizeof(code_tk);
	//add
	add();
	return 1;
}

int execute::mul(){
	ip += sizeof(code_tk);
	// go back to type
	is -= sizeof(char);
	char type1 = *(stack + is);

	int val1 = 0;
	char valc;
	float valf;
	char type2;
	switch (type1){
	case 'I':
		// go to int value and copy
		is -= sizeof(int);
		val1 = *(int*)(stack + is);

		// go to type of next val
		is -= sizeof(char);
		type2 = *(stack + is);

		// ad if they are char or 
		switch (type2){
		case 'I':
			// go  to int value
			is -= sizeof(int);

			// copy int vakue
			*(int*)(stack + is) = val1 * *(int*)(stack + is);
			// go ahead to type value
			is += sizeof(int);

			// update type
			*(stack + is) = 'I';
			is += sizeof(char);
			break;
		case 'C':
			// go back to char value
			is -= sizeof(char);

			// co[py int vakue
			*(stack + is) = val1 * *(stack + is);

			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'C';
			is += sizeof(char);

			break;
		case 'F':
			// go back to char value
			is -= sizeof(float);

			// co[py int vakue
			*(float*)(stack + is) = (float)val1 * *(float*)(stack + is);

			// go ahead to type value
			is += sizeof(float);

			// update type
			*(stack + is) = 'F';
			is += sizeof(char);

			break;
		default:
			error("type error", " ", " ");
			exit(0);
		}
		break;
	case 'C':
		// 1st val is char
		is -= sizeof(char);
		// copy char
		valc = *(stack + is);
		is -= sizeof(char);

		type2 = *(stack + is);
		// ad if they are char or 
		switch (type2){
		case 'I':
			// go  to int value
			is -= sizeof(int);

			// copy int vakue+char
			*(stack + is) = valc * *(int*)(stack + is);
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'C';
			is += sizeof(char);
			break;
		case 'C':
			// go back to char value
			is -= sizeof(char);

			// co[py char vakue	 
			*(stack + is) = valc * *(stack + is);

			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'C';
			is += sizeof(char);
			break;
		case 'F':
			// go back to char value
			is -= sizeof(float);

			// co[py int vakue
			*(float*)(stack + is) = (float)valc * *(float*)(stack + is);

			// go ahead to type value
			is += sizeof(float);

			// update type
			*(stack + is) = 'F';
			is += sizeof(char);
			break;
		default:
			error("type error", " ", " ");
			exit(0);
		}
		break;
	case 'F':
		// 1st val is float
		is -= sizeof(float);
		// copy char
		valf = *(float*)(stack + is);

		is -= sizeof(char);
		type2 = *(stack + is);

		// ad if they are char or 
		switch (type2){
		case 'I':
			// go  to int value
			is -= sizeof(int);

			// copy int vakue+char
			*(float*)(stack + is) = valf * *(int*)(stack + is);
			// go ahead to type value
			is += sizeof(float);

			// update type
			*(stack + is) = 'F';
			is += sizeof(char);
			break;
		case 'C':
			// go back to char value
			is -= sizeof(char);

			// co[py char vakue
			*(float*)(stack + is) = valf * *(stack + is);

			// go ahead to type value
			is += sizeof(float);

			// update type
			*(stack + is) = 'F';
			is += sizeof(char);
			break;
		case 'F':
			// go back to char value
			is -= sizeof(float);

			// co[py int vakue
			*(float*)(stack + is) = valf * *(float*)(stack + is);

			// go ahead to type value
			is += sizeof(float);

			// update type
			*(stack + is) = 'F';
			is += sizeof(char);
			break;
		default:
			error("type error", " ", " ");
		}
		break;
	default:
		error("type error", " ", " ");
	}

	return 1;
}

int execute::div(){
	is -= sizeof(char);
	char type = *(stack + is);
	switch (type){
	case 'I':
		is -= sizeof(int);
		*(float*)(stack + is) = 1/ (*(int*)(stack + is));
		is += sizeof(float);
		*(stack + is) = 'F';
		is += sizeof(char);
		break;
	case 'F':
		is -= sizeof(int);
		*(float*)(stack + is) = 1/(*(float*)(stack + is));
		is += sizeof(float);
		is += sizeof(char);
		break;
	default:
		error("type error in negation", " ", " ");
	}
	mul();
	return 1;
}

int execute::and(){
	ip += sizeof(code_tk);
	// go back to type
	is -= sizeof(char);
	char type1 = *(stack + is);
	char type2;
	bool val1;
	if (type1 == 'B'){
		is -= sizeof(bool);
		val1 = *(bool*)(stack + is);

		// go to type of next val
		is -= sizeof(char);
		type2 = *(stack + is);
		
		// ad if they are char or 
		if (type2 == 'B'){
			// copy int vakue
			*(bool*)(stack + is) = val1 && *(bool*)(stack+is);

		}
		else{
			error("COmparision type error ", type2, type1);
				exit(0);
		}

	}
	else{
		error("COmparision type error ", " ", type1);
		exit(0);
	}
	return 1;
}

int execute::neg(){
	ip += sizeof(code_tk);
	is -= sizeof(char);
	char type = *(stack+is);
	switch (type){
	case 'I':
		is -= sizeof(int);
		*(int*)(stack + is) = -*(int*)(stack + is);
		is += sizeof(int);
		is += sizeof(char);
		break;
	case 'F':
		is -= sizeof(int);
		*(float*)(stack + is) = -*(float*)(stack + is);
		is += sizeof(float);
		is += sizeof(char);
		break;
	default:
		error("type error in negation", " ", " ");
	}

	return 1;
}

int execute::not(){
	ip += sizeof(code_tk);
	is -= sizeof(char);

	char type = *(stack + is);

	if (type == 'B'){
		is -= sizeof(char);
		*(bool*)(stack + is) = !(*(bool*)(stack + is));
		is += sizeof(char);
		is += sizeof(char);
	}
	else{
		error("cant negate not bool ", type, " ");
		exit(0);
	}
	return 1;
}

int execute::or(){
	ip += sizeof(code_tk);
	// go back to type
	is -= sizeof(char);
	char type1 = *(stack + is);
	char type2;
	bool val1;
	if (type1 == 'B'){
		is -= sizeof(bool);
		val1 = *(bool*)(stack + is);

		// go to type of next val
		is -= sizeof(char);
		type2 = *(stack + is);

		// ad if they are char or 
		if (type2 == 'B'){
			// copy int vakue
			*(bool*)(stack + is) = val1 || *(bool*)(stack + is);

		}
		else{
			error("COmparision type error ", type2, type1);
			exit(0);
		}

	}
	else{
		error("COmparision type error ", " ", type1);
		exit(0);
	}
	return 1;
}

int execute::less(){
	ip += sizeof(code_tk);
	// go back to type
	is -= sizeof(char);
	char type1 = *(stack + is);

	int val1 = 0;
	char valc;
	float valf;
	char type2;
	switch (type1){
	case 'I':
		// go to int value and copy
		is -= sizeof(int);
		val1 = *(int*)(stack + is);

		// go to type of next val
		is -= sizeof(char);
		type2 = *(stack + is);

		// ad if they are char or 
		switch (type2){
		case 'I':
			// go  to int value
			is -= sizeof(int);

			// copy bool vakue
			*(bool*)(stack + is) = *(int*)(stack + is) < val1;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);
			break;
		case 'C':
			// go back to char value
			is -= sizeof(char);

			// copy bool vakue
			*(bool*)(stack + is) = *(stack + is) < val1;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);

			break;
		case 'F':
			// go back to char value
			is -= sizeof(float);

			// copy bool vakue
			*(bool*)(stack + is) = *(float*)(stack + is) < (float)val1;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);
			break;

		default:
			error("type error", type2, " ");
			exit(0);
		}
		break;
	case 'C':
		// 1st val is char
		is -= sizeof(char);
		// copy char
		valc = *(stack + is);
		is -= sizeof(char);

		type2 = *(stack + is);
		// ad if they are char or 
		switch (type2){
		case 'I':
			// go  to int value
			is -= sizeof(int);

			// copy bool vakue
			*(bool*)(stack + is) = *(int*)(stack + is)<valc;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);

			break;
		case 'C':
			// go back to char value
			is -= sizeof(char);

			// copy bool vakue
			*(bool*)(stack + is) = *(stack + is)<valc;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);
			break;
		case 'F':
			// go back to char value
			is -= sizeof(float);

			// copy bool vakue
			*(bool*)(stack + is) = *(float*)(stack + is) <valc;;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);

			break;
		default:
			error("type error", " ", " ");
			exit(0);
		}
		break;
	case 'F':
		// 1st val is float
		is -= sizeof(float);
		// copy char
		valf = *(float*)(stack + is);

		is -= sizeof(char);
		type2 = *(stack + is);

		// ad if they are char or 
		switch (type2){
		case 'I':
			// go  to int value
			is -= sizeof(int);

			// copy bool vakue
			*(bool*)(stack + is) =  *(int*)(stack + is)< valf;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);

			break;
		case 'C':
			// go back to char value
			is -= sizeof(char);

			// copy bool vakue
			*(bool*)(stack + is) = *(stack + is)< valf;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);
			break;
		case 'F':
			// go back to char value
			is -= sizeof(float);

			// copy bool vakue
			*(bool*)(stack + is) = *(float*)(stack + is)< valf;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);
			break;
		default:
			error("type error", " ", " ");
			exit(0);
		}
		break;
	default:
		error("type error", " ", " ");
		exit(0);
	}
	return 1;
}

int execute::less_eql(){
	ip += sizeof(code_tk);
	// go back to type
	is -= sizeof(char);
	char type1 = *(stack + is);

	int val1 = 0;
	char valc;
	float valf;
	char type2;
	switch (type1){
	case 'I':
		// go to int value and copy
		is -= sizeof(int);
		val1 = *(int*)(stack + is);

		// go to type of next val
		is -= sizeof(char);
		type2 = *(stack + is);

		// ad if they are char or 
		switch (type2){
		case 'I':
			// go  to int value
			is -= sizeof(int);

			// copy bool vakue
			*(bool*)(stack + is) = *(int*)(stack + is) <= val1;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);
			break;
		case 'C':
			// go back to char value
			is -= sizeof(char);

			// copy bool vakue
			*(bool*)(stack + is) = *(stack + is) <= val1;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);

			break;
		case 'F':
			// go back to char value
			is -= sizeof(float);

			// copy bool vakue
			*(bool*)(stack + is) = *(float*)(stack + is) <= (float)val1;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);
			break;

		default:
			error("type error", type2, " ");
			exit(0);
		}
		break;
	case 'C':
		// 1st val is char
		is -= sizeof(char);
		// copy char
		valc = *(stack + is);
		is -= sizeof(char);

		type2 = *(stack + is);
		// ad if they are char or 
		switch (type2){
		case 'I':
			// go  to int value
			is -= sizeof(int);

			// copy bool vakue
			*(bool*)(stack + is) = *(int*)(stack + is) <= valc;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);

			break;
		case 'C':
			// go back to char value
			is -= sizeof(char);

			// copy bool vakue
			*(bool*)(stack + is) = *(stack + is) <= valc;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);
			break;
		case 'F':
			// go back to char value
			is -= sizeof(float);

			// copy bool vakue
			*(bool*)(stack + is) = *(float*)(stack + is) <= valc;;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);

			break;
		default:
			error("type error", " ", " ");
			exit(0);
		}
		break;
	case 'F':
		// 1st val is float
		is -= sizeof(float);
		// copy char
		valf = *(float*)(stack + is);

		is -= sizeof(char);
		type2 = *(stack + is);

		// ad if they are char or 
		switch (type2){
		case 'I':
			// go  to int value
			is -= sizeof(int);

			// copy bool vakue
			*(bool*)(stack + is) = *(int*)(stack + is) <= valf;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);

			break;
		case 'C':
			// go back to char value
			is -= sizeof(char);

			// copy bool vakue
			*(bool*)(stack + is) = *(stack + is) <= valf;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);
			break;
		case 'F':
			// go back to char value
			is -= sizeof(float);

			// copy bool vakue
			*(bool*)(stack + is) = *(float*)(stack + is) <= valf;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);
			break;
		default:
			error("type error", " ", " ");
			exit(0);
		}
		break;
	default:
		error("type error", " ", " ");
		exit(0);
	}
	return 1;
}

int execute::greater(){
	ip += sizeof(code_tk);
	// go back to type
	is -= sizeof(char);
	char type1 = *(stack + is);

	int val1 = 0;
	char valc;
	float valf;
	char type2;
	switch (type1){
	case 'I':
		// go to int value and copy
		is -= sizeof(int);
		val1 = *(int*)(stack + is);

		// go to type of next val
		is -= sizeof(char);
		type2 = *(stack + is);

		// ad if they are char or 
		switch (type2){
		case 'I':
			// go  to int value
			is -= sizeof(int);

			// copy bool vakue
			*(bool*)(stack + is) = *(int*)(stack + is) > val1;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);
			break;
		case 'C':
			// go back to char value
			is -= sizeof(char);

			// copy bool vakue
			*(bool*)(stack + is) = *(stack + is) > val1;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);

			break;
		case 'F':
			// go back to char value
			is -= sizeof(float);

			// copy bool vakue
			*(bool*)(stack + is) = *(float*)(stack + is) > (float)val1;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);
			break;

		default:
			error("type error", type2, " ");
			exit(0);
		}
		break;
	case 'C':
		// 1st val is char
		is -= sizeof(char);
		// copy char
		valc = *(stack + is);
		is -= sizeof(char);

		type2 = *(stack + is);
		// ad if they are char or 
		switch (type2){
		case 'I':
			// go  to int value
			is -= sizeof(int);

			// copy bool vakue
			*(bool*)(stack + is) = *(int*)(stack + is)>valc;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);

			break;
		case 'C':
			// go back to char value
			is -= sizeof(char);

			// copy bool vakue
			*(bool*)(stack + is) = *(stack + is)>valc;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);
			break;
		case 'F':
			// go back to char value
			is -= sizeof(float);

			// copy bool vakue
			*(bool*)(stack + is) = *(float*)(stack + is) >valc;;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);

			break;
		default:
			error("type error", " ", " ");
			exit(0);
		}
		break;
	case 'F':
		// 1st val is float
		is -= sizeof(float);
		// copy char
		valf = *(float*)(stack + is);

		is -= sizeof(char);
		type2 = *(stack + is);

		// ad if they are char or 
		switch (type2){
		case 'I':
			// go  to int value
			is -= sizeof(int);

			// copy bool vakue
			*(bool*)(stack + is) = *(int*)(stack + is)> valf;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);

			break;
		case 'C':
			// go back to char value
			is -= sizeof(char);

			// copy bool vakue
			*(bool*)(stack + is) = *(stack + is)> valf;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);
			break;
		case 'F':
			// go back to char value
			is -= sizeof(float);

			// copy bool vakue
			*(bool*)(stack + is) = *(float*)(stack + is)> valf;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);
			break;
		default:
			error("type error", " ", " ");
			exit(0);
		}
		break;
	default:
		error("type error", " ", " ");
		exit(0);
	}
	return 1;
}

int execute::greater_eql(){
	ip += sizeof(code_tk);
	// go back to type
	is -= sizeof(char);
	char type1 = *(stack + is);

	int val1 = 0;
	char valc;
	float valf;
	char type2;
	switch (type1){
	case 'I':
		// go to int value and copy
		is -= sizeof(int);
		val1 = *(int*)(stack + is);

		// go to type of next val
		is -= sizeof(char);
		type2 = *(stack + is);

		// ad if they are char or 
		switch (type2){
		case 'I':
			// go  to int value
			is -= sizeof(int);

			// copy bool vakue
			*(bool*)(stack + is) = *(int*)(stack + is) >= val1;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);
			break;
		case 'C':
			// go back to char value
			is -= sizeof(char);

			// copy bool vakue
			*(bool*)(stack + is) = *(stack + is) >= val1;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);

			break;
		case 'F':
			// go back to char value
			is -= sizeof(float);

			// copy bool vakue
			*(bool*)(stack + is) = *(float*)(stack + is) >= (float)val1;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);
			break;

		default:
			error("type error", type2, " ");
			exit(0);
		}
		break;
	case 'C':
		// 1st val is char
		is -= sizeof(char);
		// copy char
		valc = *(stack + is);
		is -= sizeof(char);

		type2 = *(stack + is);
		// ad if they are char or 
		switch (type2){
		case 'I':
			// go  to int value
			is -= sizeof(int);

			// copy bool vakue
			*(bool*)(stack + is) = *(int*)(stack + is) >= valc;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);

			break;
		case 'C':
			// go back to char value
			is -= sizeof(char);

			// copy bool vakue
			*(bool*)(stack + is) = *(stack + is) >= valc;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);
			break;
		case 'F':
			// go back to char value
			is -= sizeof(float);

			// copy bool vakue
			*(bool*)(stack + is) = *(float*)(stack + is) >= valc;;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);

			break;
		default:
			error("type error", " ", " ");
			exit(0);
		}
		break;
	case 'F':
		// 1st val is float
		is -= sizeof(float);
		// copy char
		valf = *(float*)(stack + is);

		is -= sizeof(char);
		type2 = *(stack + is);

		// ad if they are char or 
		switch (type2){
		case 'I':
			// go  to int value
			is -= sizeof(int);

			// copy bool vakue
			*(bool*)(stack + is) = *(int*)(stack + is) >= valf;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);

			break;
		case 'C':
			// go back to char value
			is -= sizeof(char);

			// copy bool vakue
			*(bool*)(stack + is) = *(stack + is) >= valf;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);
			break;
		case 'F':
			// go back to char value
			is -= sizeof(float);

			// copy bool vakue
			*(bool*)(stack + is) = *(float*)(stack + is) >= valf;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);
			break;
		default:
			error("type error", " ", " ");
			exit(0);
		}
		break;
	default:
		error("type error", " ", " ");
		exit(0);
	}
	return 1;
}

int execute::eql(){
	ip += sizeof(code_tk);
	// go back to type
	is -= sizeof(char);
	char type1 = *(stack + is);

	int val1 = 0;
	char valc;
	float valf;
	char type2;
	switch (type1){
	case 'I':
		// go to int value and copy
		is -= sizeof(int);
		val1 = *(int*)(stack + is);

		// go to type of next val
		is -= sizeof(char);
		type2 = *(stack + is);

		// ad if they are char or 
		switch (type2){
		case 'I':
			// go  to int value
			is -= sizeof(int);

			// copy bool vakue
			*(bool*)(stack + is) = *(int*)(stack + is) == val1;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);
			break;
		case 'C':
			// go back to char value
			is -= sizeof(char);

			// copy bool vakue
			*(bool*)(stack + is) = *(stack + is) == val1;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);

			break;
		case 'F':
			// go back to char value
			is -= sizeof(float);

			// copy bool vakue
			*(bool*)(stack + is) = *(float*)(stack + is) == (float)val1;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);
			break;

		default:
			error("type error", type2, " ");
			exit(0);
		}
		break;
	case 'C':
		// 1st val is char
		is -= sizeof(char);
		// copy char
		valc = *(stack + is);
		is -= sizeof(char);

		type2 = *(stack + is);
		// ad if they are char or 
		switch (type2){
		case 'I':
			// go  to int value
			is -= sizeof(int);

			// copy bool vakue
			*(bool*)(stack + is) = *(int*)(stack + is) == valc;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);

			break;
		case 'C':
			// go back to char value
			is -= sizeof(char);

			// copy bool vakue
			*(bool*)(stack + is) = *(stack + is) == valc;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);
			break;
		case 'F':
			// go back to char value
			is -= sizeof(float);

			// copy bool vakue
			*(bool*)(stack + is) = *(float*)(stack + is) == valc;;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);

			break;
		default:
			error("type error", " ", " ");
			exit(0);
		}
		break;
	case 'F':
		// 1st val is float
		is -= sizeof(float);
		// copy char
		valf = *(float*)(stack + is);

		is -= sizeof(char);
		type2 = *(stack + is);

		// ad if they are char or 
		switch (type2){
		case 'I':
			// go  to int value
			is -= sizeof(int);

			// copy bool vakue
			*(bool*)(stack + is) = *(int*)(stack + is) == valf;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);

			break;
		case 'C':
			// go back to char value
			is -= sizeof(char);

			// copy bool vakue
			*(bool*)(stack + is) = *(stack + is) == valf;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);
			break;
		case 'F':
			// go back to char value
			is -= sizeof(float);

			// copy bool vakue
			*(bool*)(stack + is) = *(float*)(stack + is) == valf;
			// go ahead to type value
			is += sizeof(char);

			// update type
			*(stack + is) = 'B';
			is += sizeof(char);
			break;
		default:
			error("type error", " ", " ");
			exit(0);
		}
		break;
	default:
		error("type error", " ", " ");
		exit(0);
	}
	return 1;
}

int execute::jmp(){
	ip += sizeof(code_tk);

	int val = *(int*)(code + ip);
	ip = val;
	return 1;
}

int execute::eof(){
	ip += sizeof(code_tk);
	return 1;
}



/////////incomplete/////////////////////
///////////////////////////////////////////
//////////////////////////////

int execute::pos(){
	ip += sizeof(code_tk);
	return 1;
}

int execute::jfalse(){
	ip += sizeof(code_tk);
	int pos = *(int*)(code + ip);
	ip += sizeof(int);

	// go to bool
	is -= sizeof(char);
	char type = *(stack + is);

	if (type != 'B'){
		error(" bad condition for loop ", type, "not boolean");
		exit(0);
	}

	// go to bool
	is -= sizeof(char);
	bool val = *(bool*)(stack + is);

	// if stack has true
	if (val){
		ip = pos;
	}

	return 1;
}
