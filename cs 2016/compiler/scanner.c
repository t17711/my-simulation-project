
// if there is end of file or error stop program
unsigned get_eof		(struct token* token_list, int j)
{
	//printf("get_eof");
	token_list[j].name = TK_EOF;
	return 0;
}

unsigned identifier		(char *buffer,int i, struct token* token_list, int j)
{
	//printf("identifier");
	// i made it so that we enter this function if there is valid for identifier
	char curr;
	token_list[j].name = TK_ID;	

	IDENTIFIER_LOOKUP:

	curr	= buffer[i];

	if (curr==EOF || curr <= 32)
	{
	// if space after an char then we say the value is identifier
		check_keyword (&token_list[j]);
		return get_token(buffer, i + 1 , token_list, j+1);
	}
	else if // valif names are alphabet nmbers and underscore, only alphabet starts
			(('A' <= curr && curr <= 'Z')	||
			curr == '_' 			|| 
			(curr >= '0' && curr <= '9'))
	{	
		string_copy(token_list[j].value, &(curr));
		i++;
		goto IDENTIFIER_LOOKUP;
	}
	else 
	{
		check_keyword (&token_list[j]);
		return get_token(buffer, i, token_list, j+1);
	}
}

unsigned exp_token		(char *buffer,int i, struct token* token_list, int j)
{
	//printf("double\n");

	token_list[j].name = TK_DOUBLE;  // ist take everything as int
	char curr;
	int temp = 0;
	curr = buffer[i];

	while('0' <= curr && curr <= '9')
	{
		temp = temp *10 + (curr-'0');
		curr = buffer[++i];
	}
	token_list[j].exp = token_list[j].exp +temp;
	if (curr <= 32 || curr == ';')
		return get_token(buffer, i+1, token_list, j+1);
	
	else 
	{
		printf("bad double: %c\n", curr);
		return get_eof(token_list, j+1);
	}
}
unsigned double_token	(char *buffer,int i, struct token* token_list, int j)
{
	//printf("double\n");

	token_list[j].name = TK_DOUBLE;  // ist take everything as int
	char curr;
	curr = buffer[i];
	token_list[j].exp = 0; //  0.1 decimal is 1* 10^-1
	while('0' <= curr && curr <= '9')
	{
		token_list[j].dec  = token_list[j].dec *10 + (curr-'0');
		token_list[j].exp--;
		curr = buffer[++i];
	}
	if (curr == 'E')
		return exp_token(buffer , i+1, token_list, j);
	
	else if (curr <= 32 || curr == ';')
		return get_token(buffer, i+1, token_list, j+1);
	else 
	{
		printf("bad double: %c\n", curr);
		return get_eof(token_list, j+1);
	}
}
	
unsigned digit_token		(char *buffer,int i, struct token* token_list, int j )
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
		token_list[j].val = token_list[j].val*10 + (curr-'0');
		curr = buffer[++i];
	}
	
	if (curr == '.' )
		return double_token(buffer , i+1, token_list, j);
	else if (curr == 'E')
		return exp_token(buffer , i+1, token_list, j);

	else if (curr <=32 || curr == ';') 
	{
		token_list[j].name = TK_INT;  // ist take everything as int
		return get_token(buffer, i+1, token_list , j+1);
	}
	else
	{
		printf("Error in double integer: %c\n", curr);
		get_eof(token_list, j);
	}
		
}

unsigned operator_token		(char *buffer,int i, struct token* token_list, int j)
{
	//printf("operator_token\n");
	char curr;
	RESTART:
	curr = buffer[i++];
	switch (curr){
		/* defined 
		{, }, (, ), +, -, *, /, >, < , =, ?, ; ,: 
		#
		*/
		case '{':	
			token_list[j++].name = TK_BEGIN;
			return get_token(buffer, i, token_list, j);
		case '}':	
			token_list[j++].name = TK_END;
			return get_token(buffer, i, token_list, j);
		case '(':	
			token_list[j++].name = TK_OPEN;
			return get_token(buffer, i, token_list, j);
		case ')':	
			token_list[j++].name = TK_CLOSE;
			return get_token(buffer, i, token_list, j);
		case '+':	
			token_list[j++].name = TK_PLUS;
			return get_token(buffer, i, token_list, j);
		case '-':	
			token_list[j++].name = TK_MINUS;
			return get_token(buffer, i, token_list, j);
		case '*':	
			token_list[j++].name = TK_MUL;
			return get_token(buffer, i, token_list, j);
		case '/':	
		// check for comment
			if (buffer [i] == '/') 
				return comment(buffer, i+1, token_list, j, 0);
			else if (buffer [i] == '*') 
				return comment(buffer, i+1, token_list, j, 1);				
			else
			{
				token_list[j++].name = TK_DIV;
			}				
		case '>':
			if (buffer[i]== '=')
			{
				token_list[j++].name = TK_GREATER_EQUAL;
				i++;
			}
			else
			{
				token_list[j++].name = TK_GREATER;
			}
			return get_token(buffer, i, token_list, j);
		case '<':
			if (buffer[i]== '=')
			{
				token_list[j++].name = TK_LESS_EQUAL;
				i++;
			}
			else
			{
				token_list[j++].name = TK_LESS;
			}
			return get_token(buffer, i, token_list, j);
		case '=':	
			token_list[j++].name = TK_EQUAL;
			return get_token(buffer, i, token_list, j);
		case '?':
			token_list[j++].name = TK_QUES;
			return get_token(buffer, i, token_list, j);
			
		case ';':
			token_list[j++].name = TK_SEMICOLON;
			return get_token(buffer, i, token_list, j);
			
		case ',':
			token_list[j++].name = TK_COMMA;
			return get_token(buffer, i, token_list, j);

		case '#':
			return hash_token(buffer, i, token_list, j);
		case '.':
			return get_token(buffer, i, token_list, j);
		case '"':
			return get_string_token(buffer, i , token_list, j);
		default:
			printf("incorrect operator %c \n", curr);
			return get_eof(token_list, j);

	}
}

unsigned comment		(char *buffer,int i, struct token* token_list, int j , int type)
{
	//printf("comment\n");
	if (type == 0)
		while (buffer[i] != EOF && buffer[i] != '\n') i++;
	
	else if (type == 1){

		while (buffer[i] != EOF){	
			if (buffer[i]== EOF) return get_eof(token_list, j);
			else if (buffer[i++] == '*' && buffer[i] == '/') break;
		}
	}	
	return get_token(buffer , i + 1 , token_list , j);

}

unsigned include_token	(char *buffer,int i, struct token* token_list, int j, int type)
{
	//printf("include_token\n");
	token_list[j].name =  TK_INCLUDE;
	char comparer, curr;
	if (type == 0) 
		comparer = '"';
	else if (type == 1)
		comparer = '>';
	 // go inside include
	while (buffer[i]!=comparer && buffer[i] != EOF){
		curr = buffer[i++];
		string_copy(token_list[j].value,&curr );
	}
	if (buffer[i] == EOF)
	{
		printf("bad include syntax. no closing %c \n", comparer);
		return get_eof(token_list, j+1);
	}
	else
	{
		return  get_token(buffer, i +1, token_list, j+1);
	}
}

unsigned hash_token	(char *buffer,int i, struct token* token_list, int j)
{
//	printf("hash_token\n");
	if (buffer[i++]	== 'I'	&& 
		buffer[i++] == 'N'	&& 
		buffer[i++] == 'C'	&& 
		buffer[i++] == 'L'	&& 
		buffer[i++] == 'U'	&&
		buffer[i++] == 'D'	&& 
		buffer[i++] == 'E'	)
		{
			while (buffer[i]!=EOF && buffer[i] != '"' && buffer[i] != '<') 
				i++;
			if(buffer[i]==EOF) 
				return get_eof(token_list, j);
			else if (buffer[i]=='"')
				return include_token(buffer, i+1, token_list, j, 0);	
			else if (buffer[i]=='<')
				return include_token(buffer, i+1, token_list, j, 1);	
		}
	else {
		printf("bad # ");
		return get_eof(token_list, j);
	}
}


unsigned get_token		(char *buffer, int i, struct token* token_list , int j)
{	
// this is start of a token
//printf("get_token\n");
char curr;
LOOP:
	curr = buffer[i];
	if (curr == EOF)  return get_eof(token_list, j);

	else if (curr <=32) { // skip spaces
		i++;
		goto LOOP;
	}
	else if ('A' <= curr && curr <= 'z')
	{		
		return identifier(buffer, i, token_list, j); // tail recursion to identifier check
	}
	else if ('0' <= curr && curr <= '9')
		return digit_token(buffer,i, token_list, j);
	else if (curr > 32 && !('A' <= curr && curr <= 'z') && !('0' <= curr && curr <= '9') )
		return operator_token(buffer,i, token_list, j);
	
}

/////////////////////////////////////////////
/* main function for scanner
	* It checks what characters are and sends to functions to get token
	
	*/

unsigned get_string_token(char *buffer,int i, struct token* token_list, int j)
{
	token_list[j].name = TK_STRING;  // ist take everything as int
	char curr;
	curr = buffer[i];
	while(curr != '"')
	{
		if (curr == '\0')
		{
			printf("bad string\n");
			return get_eof(token_list, j);
		} 
		string_copy(token_list[j].value, &curr);
		curr = buffer[++i];
	}
	
	return get_token(buffer, i+1, token_list, j+1);
	
}