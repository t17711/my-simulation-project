#pragma warning(disable:4996)
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <map>

#include "token.h"
using namespace std;

class scanner{
public:
	char *buffer;
	FILE * pfile;
	token **token_list;// = new token*[result];
	size_t result;
	int i;
	int j;
	std::map<string,token_name> key_table;
	int key_n;
	scanner(){

		pfile = fopen("test.txt", "rb"); // rb is read as binary
		if (!pfile) {
			perror("file opening failed");
			return;
		}

		// allocate size


		fseek(pfile, 8, SEEK_END);  // go to end
		size_t size = ftell(pfile); //  get size
		rewind(pfile);  // go to begining

		buffer = (char*)malloc(size + 1);

	
		if (buffer == NULL) { fputs("Memory error", stderr); exit(2); }

		// copy the file into the buffer:
		result = std::fread(buffer, 1, size, pfile);
		printf("file size is : %ld\n", size);
		printf("file size is : %ld\n", result);

		token_list = new token*[result];

		buffer[result] = EOF;
		size_t t = 0;
		for (char *p = buffer; *p; p++)
			*p = toupper(*p);

		while (t<result) cout << buffer[t++];

		// create a list of tokens
		get_key_table();
		for (size_t i = 0; i < result; ++i){
			token_list[i] = new token();
		}
		
	
		cout << "\nfinish" << endl;
	}

	~scanner(){
		delete[] token_list;
		fclose(pfile);
		free(buffer);

	}

	void print(){
		int t = 0;
		while (token_list[t]->name != TK_EOF) {
			token_list[t]->print();
			t++;
		}

	}

	void get_eof()
	{
		//printf("get_eof");
		token_list[j]->name = TK_EOF;
	}

	void get_key_table(){
		key_n = 6;
		key_table["FOR"] = TK_FOR;
		key_table["INT"] = TK_INT_DEF;
		key_table["FLOAT"] = TK_FLOAT_DEF;
		key_table["STRING"] = TK_STRING_DEF;
		key_table["WHILE"] = TK_WHILE;
		key_table["BOOL"] = TK_BOOL_DEF;
		key_table["RETURN"] = TK_RETURN;
	}

	void check_keyword(token* tk)
	{
		token_name t = key_table[tk->id];
		if (t != TK) tk->name = t;
	}

	void get_token()
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
		else if (curr > 32 && !('A' <= curr && curr <= 'Z') && !('0' <= curr && curr <= '9')){
			operator_token();
			return;
		}

	}

	void identifier()
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

	void operator_token()
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
			}
			else
			{
				token_list[j++]->name = TK_GREATER;
			}
			i++;
			break;
		case '<':
			if (buffer[i] == '=')
			{
				token_list[j++]->name = TK_LESS_EQUAL;
			}
			else
			{
				token_list[j++]->name = TK_LESS;
			}

			i++;
			break;
		case '=':
			token_list[j++]->name = TK_EQUAL;
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
		case '.':
			i++;
			break;
		case '"':
		//	get_string

			get_string_token();
			return;
		default:
			printf("incorrect operator %c \n", curr);
			{
				get_eof();
				return;
			}
		}
		get_token();
	}

	void digit_token()
	{
		// int are int
		// decimals are 2 int
		// there is also exp portion as power of 10
		// float later
		//printf("digit_token\n");
		char curr;
		curr = buffer[i];
		//printf("%c\n", curr);
		while ('0' <= curr && curr <= '9')
		{
			token_list[j]->int_value = token_list[j]->int_value * 10 + (curr - '0');
			curr = buffer[++i];
		}

		if (curr == '.')
		{
			i++;
			double_token();
			return;
		}
		else if (curr == 'E')
		{
			i++;
			exp_token();
			return;
		}

		else if (curr <= 32 || curr == ';')
		{
			token_list[j++]->name = TK_INT;  // ist take everything as int
			i++;
			get_token();
			return;
		}
		else
		{
			printf("Error in double integer: %c\n", curr);
			get_eof();
			return;
		}

	}

	void double_token()
	{
		//printf("double\n");

		token_list[j]->name = TK_FLOAT;  // ist take everything as int
		char curr;
		curr = buffer[i];
		token_list[j]->exp = 0; //  0.1 decimal is 1* 10^-1
		while ('0' <= curr && curr <= '9')
		{
			token_list[j]->float_value = token_list[j]->float_value * 10 + (curr - '0');
			token_list[j]->exp--;
			curr = buffer[++i];
		}
		if (curr == 'E')
		{
			i++;
			exp_token();
			return;
		}
		else if (curr <= 32 || curr == ';')
		{
			i++;
			j++;
			get_token();
			return;
		}
		else
		{
			printf("bad double: %c\n", curr);
			j++;
			get_eof();
			return;
		}
	}

	void exp_token()
	{
		//printf("double\n");

		token_list[j]->name = TK_FLOAT;  // ist take everything as int
		char curr;
		int temp = 0;
		curr = buffer[i];

		while ('0' <= curr && curr <= '9')
		{
			temp = temp * 10 + (curr - '0');
			curr = buffer[++i];
		}
		token_list[j]->exp = token_list[j]->exp + temp;
		if (curr <= 32 || curr == ';')
		{
			i++;
			j++;
			get_token();
			return;
		}
		else
		{
			printf("bad double: %c\n", curr);
			{
				j++;
				get_eof();
				return;
			}
		}
	}

	void hash_token()
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

	void include_token(int type)
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

	void get_string_token()
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

	void comment( int type)
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

};