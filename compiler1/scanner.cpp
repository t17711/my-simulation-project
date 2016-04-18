#include "header.h"

// if there is end of file or error stop program
void get_eof( token* token_list, int j)
{
	//printf("get_eof");
	token_list[j].name = TK_EOF;
}

void identifier(char *buffer, int i,  token* token_list, int j)
{
	//printf("identifier");
	// i made it so that we enter this function if there is valid for identifier
	char curr;
	token_list[j].name = TK_ID;

IDENTIFIER_LOOKUP:

	curr = buffer[i];

	if (curr == EOF || curr <= 32)
	{
		// if space after an char then we say the value is identifier
		token t = token_list[j];
		check_keyword(t);
		get_token(buffer, i + 1, token_list, j + 1);
		return;
	}
	else if // valif names are alphabet nmbers and underscore, only alphabet starts
		(('A' <= curr && curr <= 'Z') ||
		curr == '_' ||
		(curr >= '0' && curr <= '9'))
	{
		token_list[j].value+=curr;
		i++;
		goto IDENTIFIER_LOOKUP;
	}
	else
	{
		check_keyword(token_list[j]);
		get_token(buffer, i, token_list, j + 1);
		return;
	}
}

void exp_token(char *buffer, int i,  token* token_list, int j)
{
	//printf("double\n");

	token_list[j].name = TK_DOUBLE;  // ist take everything as int
	char curr;
	int temp = 0;
	curr = buffer[i];

	while ('0' <= curr && curr <= '9')
	{
		temp = temp * 10 + (curr - '0');
		curr = buffer[++i];
	}
	token_list[j].exp = token_list[j].exp + temp;
	if (curr <= 32 || curr == ';')
	{
		get_token(buffer, i + 1, token_list, j + 1);
		return;
	}
	else
	{
		printf("bad double: %c\n", curr);
		{
			get_eof(token_list, j + 1);
			return;
		}
	}
}
void double_token(char *buffer, int i,  token* token_list, int j)
{
	//printf("double\n");

	token_list[j].name = TK_DOUBLE;  // ist take everything as int
	char curr;
	curr = buffer[i];
	token_list[j].exp = 0; //  0.1 decimal is 1* 10^-1
	while ('0' <= curr && curr <= '9')
	{
		token_list[j].dec = token_list[j].dec * 10 + (curr - '0');
		token_list[j].exp--;
		curr = buffer[++i];
	}
	if (curr == 'E')
	{
		exp_token(buffer, i + 1, token_list, j);
		return;
	}
	else if (curr <= 32 || curr == ';')
	{
		get_token(buffer, i + 1, token_list, j + 1);
		return;
	}
	else
	{
		printf("bad double: %c\n", curr);
		get_eof(token_list, j + 1);
		return;
	}
}

void digit_token(char *buffer, int i,  token* token_list, int j)
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
		token_list[j].val = token_list[j].val * 10 + (curr - '0');
		curr = buffer[++i];
	}

	if (curr == '.')
	{
		double_token(buffer, i + 1, token_list, j);
		return;
	}
	else if (curr == 'E')
	{
		exp_token(buffer, i + 1, token_list, j);
		return;
	}

	else if (curr <= 32 || curr == ';')
	{
		token_list[j].name = TK_INT;  // ist take everything as int
		get_token(buffer, i + 1, token_list, j + 1);
		return;
	}
	else
	{
		printf("Error in double integer: %c\n", curr);
		get_eof(token_list, j);
		return;
	}

}

void operator_token(char *buffer, int i,  token* token_list, int j)
{
	//printf("operator_token\n");
	char curr;
	curr = buffer[i++];
	switch (curr){
		/* defined
		{, }, (, ), +, -, *, /, >, < , =, ?, ; ,:
		#
		*/
	case '{':
		token_list[j++].name = TK_BEGIN;
		get_token(buffer, i, token_list, j);
		return;
	case '}':
		token_list[j++].name = TK_END;
		 get_token(buffer, i, token_list, j);
		 return;
	case '(':
		token_list[j++].name = TK_OPEN;
		 get_token(buffer, i, token_list, j);
		 return;
	case ')':
		token_list[j++].name = TK_CLOSE;
		 get_token(buffer, i, token_list, j);
		 return;
	case '+':
		token_list[j++].name = TK_PLUS;
		 get_token(buffer, i, token_list, j);
		 return;
	case '-':
		token_list[j++].name = TK_MINUS;
		 get_token(buffer, i, token_list, j);
		 return;
	case '*':
		token_list[j++].name = TK_MUL;
		 get_token(buffer, i, token_list, j);
		 return;
	case '/':
		// check for comment
		if (buffer[i] == '/'){
			comment(buffer, i + 1, token_list, j, 0);
			return;
		}
		else if (buffer[i] == '*'){
			comment(buffer, i + 1, token_list, j, 1);
			return;
		}
		else
		{
			token_list[j++].name = TK_DIV;
		}
	case '>':
		if (buffer[i] == '=')
		{
			token_list[j++].name = TK_GREATER_EQUAL;
			i++;
		}
		else
		{
			token_list[j++].name = TK_GREATER;
		}
		get_token(buffer, i, token_list, j);
		return;
	case '<':
		if (buffer[i] == '=')
		{
			token_list[j++].name = TK_LESS_EQUAL;
			i++;
		}
		else
		{
			token_list[j++].name = TK_LESS;
		}
		 get_token(buffer, i, token_list, j);
		 return;
	case '=':
		token_list[j++].name = TK_EQUAL;
		 get_token(buffer, i, token_list, j);
		 return;
	case '?':
		token_list[j++].name = TK_QUES;
		 get_token(buffer, i, token_list, j);
		 return;
	case ';':
		token_list[j++].name = TK_SEMICOLON;
		 get_token(buffer, i, token_list, j);
		 return;
	case ',':
		token_list[j++].name = TK_COMMA;
		 get_token(buffer, i, token_list, j);
		 return;
	case '#':
		 hash_token(buffer, i, token_list, j);
		 return;
	case '.':
		 get_token(buffer, i, token_list, j);
		 return;
	case '"':
		 get_string_token(buffer, i, token_list, j);
		 return;
	default:
		printf("incorrect operator %c \n", curr);
		{
			get_eof(token_list, j);
			return;
		}
	}
}

void comment(char *buffer, int i,  token* token_list, int j, int type)
{
	//printf("comment\n");
	if (type == 0)
	while (buffer[i] != EOF && buffer[i] != '\n') i++;

	else if (type == 1){

		while (buffer[i] != EOF){
			if (buffer[i] == EOF) {
				get_eof(token_list, j);
				return;
			}
			else if (buffer[i++] == '*' && buffer[i] == '/') break;
		}
	}
	get_token(buffer, i + 1, token_list, j);
	return;
}

void include_token(char *buffer, int i,  token* token_list, int j, int type)
{
	//printf("include_token\n");
	token_list[j].name = TK_INCLUDE;
	char comparer, curr;
	if (type == 0)
		comparer = '"';
	else if (type == 1)
		comparer = '>';
	// go inside include
	while (buffer[i] != comparer && buffer[i] != EOF){
		curr = buffer[i++];
		token_list[j].value += curr;
	}
	if (buffer[i] == EOF)
	{
		printf("bad include syntax. no closing %c \n", comparer);
		get_eof(token_list, j + 1);
		return;
	}
	else
	{
		get_token(buffer, i + 1, token_list, j + 1);
		return;
	}
}

void hash_token(char *buffer, int i,  token* token_list, int j)
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
			get_eof(token_list, j);	
			return;
		}
		else if (buffer[i] == '"'){
			include_token(buffer, i + 1, token_list, j, 0);
			return;
		}
		else if (buffer[i] == '<'){
			include_token(buffer, i + 1, token_list, j, 1);
			return;
	}
	}
	else {
		printf("bad # ");
		get_eof(token_list, j);
		return;
	}
}


void get_token(char *buffer, int i,  token* token_list, int j)
{
	// this is start of a token
	//printf("get_token\n");
	char curr;
LOOP:
	curr = buffer[i];
	if (curr == EOF)  {
		get_eof(token_list, j);
		return;
	}

	else if (curr <= 32) { // skip spaces
		i++;
		goto LOOP;
	}
	else if ('A' <= curr && curr <= 'z')
	{
		identifier(buffer, i, token_list, j); // tail recursion to identifier check
		return;
	}
	else if ('0' <= curr && curr <= '9'){
		digit_token(buffer, i, token_list, j);
		return;
	}
	else if (curr > 32 && !('A' <= curr && curr <= 'z') && !('0' <= curr && curr <= '9')){
		 operator_token(buffer, i, token_list, j);
		 return;
	}

}

/////////////////////////////////////////////
/* main function for scanner
* It checks what characters are and sends to functions to get token

*/

void get_string_token(char *buffer, int i,  token* token_list, int j)
{
	token_list[j].name = TK_STRING;  // ist take everything as int
	char curr;
	curr = buffer[i];
	while (curr != '"')
	{
		if (curr == '\0')
		{
			printf("bad string\n");
			return get_eof(token_list, j);
		}
		token_list[j].value += curr;
		curr = buffer[++i];
	}

	return get_token(buffer, i + 1, token_list, j + 1);

}


void check_keyword_iter(token tk, char *key, int size, token_name x)
{
	int i = 0;
	while (i<size){
		if (tk.value[i] && tk.value[i] == key[i]) i++;
		else return;
	}
	if (!tk.value[i])
	{
		tk.value = "";
		tk.name = x;
	}
}

void check_keyword(token tk)
{
	// check for for
	char f[3] = { 'F', 'O', 'R' };
	if (tk.value[0] == 'F')
		check_keyword_iter(tk, f, 3, TK_FOR);

	char i[3] = { 'I', 'N', 'T' };
	if (tk.value[0] == 'I')
		check_keyword_iter(tk, i, 3, TK_INT_DEF);

	char d[6] = { 'D', 'O', 'U', 'B', 'L', 'E' };
	if (tk.value[0] == 'D')
		check_keyword_iter(tk, d, 6, TK_DOUBLE_DEF);

	char s[6] = { 'S', 'T', 'R', 'I', 'N', 'G' };
	if (tk.value[0] == 'S')
		check_keyword_iter(tk, s, 6, TK_STRING_DEF);

	char w[5] = { 'W', 'H', 'I', 'L', 'E' };
	if (tk.value[0] == 'W')
		check_keyword_iter(tk, w, 5, TK_WHILE);

	char r[6] = { 'R', 'E', 'T', 'U', 'R', 'N' };
	if (tk.value[0] == 'R')
		check_keyword_iter(tk, r, 6, TK_RETURN);

	char b[4] = { 'B', 'O', 'O', 'L' };
	if (tk.value[0] == 'B')
		check_keyword_iter(tk, b, 4, TK_BOOL_DEF);

}
