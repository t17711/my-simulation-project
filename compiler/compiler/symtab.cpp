#include "symtab.h"

// initialize variables
symtab::symtab(int size)
{
	MAX = size;
	pos = 0;
	symarray = (char*)malloc(sizeof(char)*MAX);
	for (int i = 0; i<MAX; ++i) *(symarray + i) = 0;
}

// check if an id is in symtab
bool
symtab::check_symtab(string id){
	std::map<string, int>::iterator it;
	it = address.find(id);
	if (it != address.end())
		return true;
	else
		return false;
}

// extend symtab
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
// also add type as char to array for easier type
void symtab::insert_id(string id, char c){
	address[id] = pos;
	if (MAX / 2 < pos){ // assert that array is enpugh to hold all values
		char* temp = (char*)malloc(sizeof(char)* 2 * MAX);

		// copy values
		for (int i = 0; i < MAX; ++i) *(temp + i) = *(symarray + i);

		free(symarray);// clear old memory 
		symarray = temp; // point to new array
		MAX *= 2;

	}
	switch (c){
	case 'I':  // add space for char that defines type and then place for int value
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
	case 'P':  // add space for procedure spaces, no arguements now
		*(symarray + pos) = 'P';
		pos++;
		pos += sizeof(int); // add space for address
		break;
	}
}

void symtab::insert_addr(string id, int val, char type){
	// need to be already declared
	// taken care of that in parser
	int val2 = get_address(id);
	*(symarray + val2) = type;
	val2 += sizeof(char);
	*(int*)(symarray + val2) = val;
}

// insert array
void symtab::insert_array(string id, int size, char type){
	address[id] = pos;
	if (MAX / 2 < pos + size){ // assert that array is enpugh to hold all values
		int next_size = MAX * 2 + pos + size;
		char* temp = (char*)malloc(sizeof(char)* 2 * next_size);

		// copy values
		for (int i = 0; i < MAX; ++i) *(temp + i) = *(symarray + i);

		free(symarray);// clear old memory 
		symarray = temp; // point to new array
		MAX = next_size;
	}

	// declare this is array
	*(symarray + pos) = 'A';
	pos++;
	int low = 0;;
	int high;
	int addr;
	// only integer index, 
	// in symaray : = A, addressof start, type of element, lo ,high, lo is always 0
	//now look for how many space to store
	switch (type){
	case 'I':  // add space for char that defines type and then place for int value
		high = low + sizeof(int)*size;

		addr = pos;
		pos += sizeof(int);
		*(symarray + pos) = 'I';
		pos++;

		*(int*)(symarray + pos) = low;
		pos += sizeof(int);
		*(int*)(symarray + pos) = high;
		pos += sizeof(int);

		*(int*)(symarray + addr) = pos;
		pos += high;
		break;
	case 'C':
		high = low + sizeof(char)*size;

		addr = pos;
		pos += sizeof(int);
		*(symarray + pos) = 'C';
		pos++;

		*(int*)(symarray + pos) = low;
		pos += sizeof(int);
		*(int*)(symarray + pos) = high;
		pos += sizeof(int);

		*(int*)(symarray + addr) = pos;
		pos += high;
		break;
	case 'F':
		high = low + sizeof(float)*size;

		addr = pos;
		pos += sizeof(int);
		*(symarray + pos) = 'F';
		pos++;

		*(int*)(symarray + pos) = low;
		pos += sizeof(int);
		*(int*)(symarray + pos) = high;
		pos += sizeof(int);

		*(int*)(symarray + addr) = pos;
		pos += high;
		break;
	case 'B':
		high = low + sizeof(bool)*size;

		addr = pos;
		pos += sizeof(int);
		*(symarray + pos) = 'B';
		pos++;

		*(int*)(symarray + pos) = low;
		pos += sizeof(int);
		*(int*)(symarray + pos) = high;
		pos += sizeof(int);

		*(int*)(symarray + addr) = pos;
		pos += high;
	default:
		printf("illegal array type" + type);
	}

}

// get type of a id
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

// get address of a id
int symtab::get_address(string id){
	return address[id];
}

//. print symtab address and id
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
