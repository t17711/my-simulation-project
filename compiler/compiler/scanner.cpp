#pragma warning(disable:4996)
#include "scanner.h"

using namespace std;

scanner::scanner(){
		while (true) {
			string name;// = "test.txt";
			std::cout << " enter file name: ";
			std::cin >> name;
			const char* t = name.c_str();
			pfile = fopen(t, "rb"); // rb is read as binary
			
			if (pfile) 
				break;
			else
				error("file opening failed \n", " ", " ");


		}

		// allocate size


		fseek(pfile, 8, SEEK_END);  // go to end
		size_t size = ftell(pfile); //  get size
		rewind(pfile);  // go to begining

		buffer = (char*)malloc(size + 1);

	
		if (buffer == NULL) { error("Memory error", " ", " "); 
		exit(2); }

		// copy the file into the buffer:
		result = std::fread(buffer, 1, size, pfile);
	/*	printf("file size is : %ld\n", size);
		printf("file size is : %ld\n", result);*/

		token_list = new token*[result];

		buffer[result] = EOF;
		size_t t = 0;
		for (char *p = buffer; *p; p++){
			// dont touch string
			if (*p == '"'){
				p++;
				while (*p != '"')p++;
				p++;
			}
			else if (*p == '\''){
				p += 3;
			}
			else{
				*p = toupper(*p);
			}
		}

		//while (t<result) cout << buffer[t++];

		// create a list of tokens
		get_key_table();
		for (size_t i = 0; i < result; ++i){
			token_list[i] = new token();
		}
		
	
		//cout << "\nfinish" << endl;
	}

scanner::~scanner(){
		fclose(pfile);
		free(buffer);

	}

void
scanner::print(){
		int t = 0;
		while (token_list[t]->name != TK_EOF) {
			token_list[t]->print();
			t++;
		}
		token_list[t]->print();
		std::cout << std::endl << std::endl;
	}

void
scanner::get_eof()
	{
		//printf("get_eof");
		token_list[j]->name = TK_EOF;
	}

void
scanner::get_key_table(){
		key_table["CHAR"] = TK_CHAR_DEF;
		key_table["FOR"] = TK_FOR;
		key_table["INT"] = TK_INT_DEF;
		key_table["FLOAT"] = TK_FLOAT_DEF;
		key_table["STRING"] = TK_STRING_DEF;
		key_table["WHILE"] = TK_WHILE;
		key_table["BOOL"] = TK_BOOL_DEF;
		key_table["RETURN"] = TK_RETURN;
		key_table["PRINT"] = TK_PRINT;
		key_table["OR"] = TK_OR;
		key_table["AND"] = TK_AND;
		key_table["DO"] = TK_DO;
		key_table["TRUE"] = TK_TRUE;
		key_table["FALSE"] = TK_FALSE;
		key_table["NOT"] = TK_NOT;
		key_table["IF"] = TK_IF;
		key_table["ELSE"] = TK_ELSE;
		key_table["SWITCH"] = TK_SWITCH;
		key_table["CASE"] = TK_CASE;
		key_table["DEFAULT"] = TK_DEFAULT;

	}

void
scanner::check_keyword(token* tk)
	{
		token_name t = key_table[tk->id];
		if (t != TK) tk->name = t;
	}
//main scanner 
void
scanner::get_token()
	{
		// this is start of a token
		char curr;
		//while (i < result) token_list[i++] = new token(TK_INT, 10);
	GET_TOKEN:

		// check if char are legal';
		curr = buffer[i];

		if (curr == EOF)  {

			get_eof(); // this should exit the loop put eof on token list
			return;
		}

		else if (curr <= 32){/// skip spaces
			i++;
			goto GET_TOKEN;
		}
		else if ('A' <= curr && curr <= 'Z'){
			identifier();
			return;
		}
		else if ('0' <= curr && curr <= '9'){
			digit_token();
			return;
		}
		else {
			operator_token();
			return;
		}

	}

void
scanner::identifier()
	{
		// i made it so that we enter this function if there is valid for identifier
	
		char curr;
		token_list[j]->name = TK_ID;

	IDENTIFIER_LOOKUP:

		curr = buffer[i];

		if (curr == EOF){
			get_eof();
			return;
		}

		else if(curr <= ' '){
		    
			// if space after an char then we say the value is identifier
			
			check_keyword(token_list[j]);
			j++;
			get_token();
			return;
		}

		else if (('A' <= curr && curr <= 'Z') ||
					curr == '_' ||
					(curr >= '0' && curr <= '9')){
			// valif names are alphabet nmbers and underscore, only alphabet starts
			token_list[j]->id += curr;
			i++;
			goto IDENTIFIER_LOOKUP;
		}

		else
		{
			check_keyword(token_list[j]);
			j++;
			get_token();
			return;
		}
	}

void 
scanner::operator_token()
	{
	//	cout << "operator_token\n";

		char curr;
		curr = buffer[i++];
		switch (curr){
			/* defined
			{, }, (, ), +, -, *, /, >, < , =, ?, ; ,:
			#
			*/
		case '{':
			token_list[j++]->name = TK_BEGIN;
			break;
		case '}':
			token_list[j++]->name = TK_END;
			break;
		case '(':
			token_list[j++]->name = TK_OPEN;
			break;
		case ')':
			token_list[j++]->name = TK_CLOSE;
			break;
		case '+':
			token_list[j++]->name = TK_PLUS;
			break;
		case '-':
			token_list[j++]->name = TK_MINUS;
			break;
		case '*':
			token_list[j++]->name = TK_MUL;
			break;
		case '/':
			// check for comment
			if (buffer[i] == '/'){
				i++;
				comment(0);
				return;
			}
			else if (buffer[i] == '*'){
				i++;
				comment(1);
				return;
			}
			else
			{
				token_list[j++]->name = TK_DIV;
				break;
			}
		case '>':
			if (buffer[i] == '=')
			{
				token_list[j++]->name = TK_GREATER_EQUAL;
				i++;
			}
			else
			{
				token_list[j++]->name = TK_GREATER;
			}
			break;
		case '<':
			if (buffer[i] == '=')
			{
				token_list[j++]->name = TK_LESS_EQUAL;
				i++;
			}
			else
			{
				token_list[j++]->name = TK_LESS;
			}

			break;
		case '=':
			if (buffer[i] == '=')
			{
				token_list[j++]->name = TK_EQUAL_COMP;
				i++;
			}
			else
			{
				token_list[j++]->name = TK_EQUAL;
			}
			break;
		case '?':
			token_list[j++]->name = TK_QUES;
			break;
		case ';':
			token_list[j++]->name = TK_SEMICOLON;
			break;
		case ',':
			token_list[j++]->name = TK_COMMA;
			break;
		case '#':
			hash_token();
			return;
		case ':':
			token_list[j++]->name = TK_COLON;
			break;
		case '\'':
			//	get char
			get_char_token();
			return;

		case '"':
		//	get_string
			get_string_token();
			return;
		default:
			error("incorrect operator", " ", " ");
			exit(0);
		}
		get_token();
	}

void 
scanner::digit_token()
	{
		// int are int
		// decimals are 2 int
		// there is also exp portion as power of 10
		// float later
		//printf("digit_token\n");
		
		char curr;
		curr = buffer[i];


		while ('0' <= curr && curr <= '9')
		{
			token_list[j]->int_value = token_list[j]->int_value * 10 + (curr - '0');
			i++;
			curr = buffer[i];
		}

		if (curr == '.')
		{
			i++;
			float_token();
			return;
		}
		else if (curr == 'E')
		{
			i++;
			exp_token();
			return;
		}

		// digit cannot be followed by character, i did not do hex here maybe later
		else if (curr >= 'A' && curr <= 'Z')
		{
			printf("Error in integer: %c\n", curr);
			get_eof();
			return;
		}

		// if . or e are not found and other non numbers are then they are next token
		else
		{
			token_list[j++]->name = TK_INT;  // ist take everything as int
			get_token();
			return;

		}
	}

void 
scanner::float_token()
{
	//printf("double\n");

	token_list[j]->name = TK_FLOAT;  // ist take everything as int
	char curr;
	curr = buffer[i];
	token_list[j]->exp = 0; //  0.1 decimal is 1* 10^-1

	// 1st copy int value
	token_list[j]->float_value = static_cast<float>(token_list[j]->int_value);
	token_list[j]->int_value = 0;

	float e = 0.0;
	float pos = 1.0;
	while ('0' <= curr && curr <= '9')
	{
		e = e*10+ (curr - '0');
		curr = buffer[++i];
		pos*=10;
	}

	// add exponent part too
	token_list[j]->float_value += e/pos;

	if (curr == 'E')
	{
		i++;
		exp_token();
		return;
	}
	else if (curr <= 32 || curr == ';')
	{
		j++;
		get_token();
		return;
	}
	else
	{
		printf("bad float: %c\n", curr);
		j++;
		get_eof();
		return;
	}
}

void
scanner::exp_token()
{
	//printf("double\n");

	token_list[j]->name = TK_FLOAT;  // ist take everything as int
	char curr;
	int temp = 0;
	curr = buffer[i];

	while ('0' <= curr && curr <= '9')
	{
		temp = temp * 10 + (curr - '0');
		i++;
		curr = buffer[i];
	}
	token_list[j]->exp =  temp;
		
	if (curr <= 32 || curr == ';')
	{
		j++;
		get_token();
		return;
	}
	else
	{
		printf("bad exp: %c\n", curr);
		{
			j++;
			get_eof();
			return;
		}
	}
}

void 
scanner::hash_token()
{
	//	printf("hash_token\n");
	if (buffer[i++] == 'I'	&&
		buffer[i++] == 'N'	&&
		buffer[i++] == 'C'	&&
		buffer[i++] == 'L'	&&
		buffer[i++] == 'U'	&&
		buffer[i++] == 'D'	&&
		buffer[i++] == 'E')
	{
		while (buffer[i] != EOF && buffer[i] != '"' && buffer[i] != '<')
			i++;
		if (buffer[i] == EOF){
			get_eof();
			return;
		}
		else if (buffer[i] == '"'){
			i++;
			include_token(0);
			return;
		}
		else if (buffer[i] == '<'){
			i++; j++;
			include_token(1);
			return;
		}
	}
	else {
		printf("bad # ");
		get_eof();
		return;
	}
}

void 
scanner::include_token(int type)
{
	//printf("include_token\n");
	token_list[j]->name = TK_INCLUDE;
	char comparer, curr;
	if (type == 0)
		comparer = '"';
	else if (type == 1)
		comparer = '>';
	// go inside include
	while (buffer[i] != comparer && buffer[i] != EOF){
		curr = buffer[i++];
		token_list[j]->id += curr;
	}
	if (buffer[i] == EOF)
	{
		printf("bad include syntax. no closing %c \n", comparer);
		j++;
		get_eof();
		return;
	}
	else
	{
		i++; j++;
		get_token();
		return;
	}
}

void 
scanner::get_string_token()
{
	token_list[j]->name = TK_STRING;  // ist take everything as int
	char curr;
	curr = buffer[i];
	while (curr != '"')
	{
		if (curr == '\0')
		{
			printf("bad string\n");
			return get_eof();
		}
		token_list[j]->id += curr;
		curr = buffer[++i];
	}
	i++; j++;
	return get_token();

}
	
void
scanner::get_char_token(){
	//std::cout << buffer[i] << endl;
	token_list[j]->name = TK_CHAR;  
	// any thing can be char

	if (buffer[i] == '\\'){
		i++;
		switch (buffer[i]){
		case 'n':
			token_list[j]->char_val = '\n';
			break;
		case 't':
			token_list[j]->char_val = '\t';
			break;
		default:
			error("bad string operator ", buffer[i], " ");
			exit(0);
		}
		i++;
	}
	else{
		token_list[j]->char_val = buffer[i];
		i++;
	}
		
	if (buffer[i]!= '\''){
		error("char not closed", buffer[i], " ");
		exit(0);
	}
	else{
		i++;
		j++;
		get_token();
	}
}
	
void 
scanner::comment( int type)
{
	//printf("comment\n");
	if (type == 0)
	while (buffer[i] != EOF && buffer[i] != '\n') i++;

	else if (type == 1){

		while (buffer[i] != EOF){
			if (buffer[i] == EOF) {
				get_eof();
				return;
			}
			else if (buffer[i++] == '*' && buffer[i] == '/') break;
		}
	}
	i++;
	//no j++ since it skips the operator
	get_token();
	return;
}
