unsigned clear(char* array)
{
	int i=0;
	while(array[i])
		array[i++]='\0';
}

unsigned check_keyword_iter (struct token *tk , char *key, int size, token_name x)
{
	int i = 0;
	while(i<size){
		if (tk->value[i] && tk->value[i] == key [i]) i++;
		else return 0;
	}
	if (!tk->value[i])
	{
		clear(tk->value);
		tk->name = x;
	}
	return 1;
}

unsigned check_keyword(struct token *tk)
{
	// check for for
	char f [3] = {'F' , 'O', 'R'};
	if (tk->value[0] == 'F')
		check_keyword_iter		(tk, f, 3 , TK_FOR);
	
	char i[3] = {'I', 'N' ,'T'};
	if (tk->value[0] == 'I')
		check_keyword_iter		(tk, i, 3, TK_INT_DEF);
	
	char d[6] = {'D', 'O' ,'U', 'B', 'L', 'E'};
	if (tk->value[0] == 'D')
		check_keyword_iter		(tk, d, 6 , TK_DOUBLE_DEF);
	
	char s[6] = {'S', 'T' ,'R', 'I', 'N', 'G'};
	if (tk->value[0] == 'S')
		check_keyword_iter		(tk, s, 6 , TK_STRING_DEF);

	char w[5] = {'W', 'H' ,'I', 'L', 'E'};
	if (tk->value[0] == 'W')
		check_keyword_iter		(tk, w, 5 , TK_WHILE);
	
	char r[6] = {'R', 'E' ,'T', 'U', 'R', 'N'};
	if (tk->value[0] == 'R')
		check_keyword_iter		(tk, r, 6 , TK_RETURN);
	
	char b[4] = {'B', 'O' ,'O', 'L'};
	if (tk->value[0] == 'B')
		check_keyword_iter		(tk, b, 4 , TK_BOOL_DEF);

	return 1;
}
