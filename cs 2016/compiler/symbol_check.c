unsigned clear(char* array)
{
	int i=0;
	while(array[i])
		array[i++]='\0';
}

unsigned check_keyword(struct token *tk)
{
	// check for for
	if (tk->value[0] && tk->value[0] =='F')
	{
		if (tk->value[1] && tk->value[1] =='O')
		if (tk->value[2] && tk->value[2] =='R')
			if (!tk->value[3])
			{
			clear(tk->value);
			tk->name = TK_FOR;
			return 1;
			}
	}
	
	// check for while
	if(tk->value[0] && tk->value[0] =='W')
	{
		if(tk->value[1] && tk->value[1] =='H')
		if(tk->value[2] && tk->value[2] =='I')
		if(tk->value[3] && tk->value[3] =='L')
		if(tk->value[4] && tk->value[4] =='E')
		if (!tk->value[5])
			{
			clear(tk->value);
			tk->name = TK_WHILE;
			return 1;
			}
	}
	
	// check for int definition	
	if	(tk->value[0] && tk->value[0] =='I')
	{
		if(tk->value[1] && tk->value[1] =='N')
		if(tk->value[2] && tk->value[2] =='T')
		if (!tk->value[3])
			{
			clear(tk->value);
			tk->name = TK_INT_DEF;
			return 1;
			}
	}
	// check for double definition
	if		(tk->value[0] && tk->value[0] =='D')
	{
		if(tk->value[1] && tk->value[1] =='O')
		if(tk->value[2] && tk->value[2] =='U')
		if(tk->value[3] && tk->value[3] =='B')
		if(tk->value[4] && tk->value[4] =='L')
		if(tk->value[5] && tk->value[5] =='E')
		if(!tk->value[6])
			{
			clear(tk->value);
			tk->name = 	TK_DOUBLE_DEF;
			return 1;
			}
	}
	//check for return
	if(tk->value[0] && tk->value[0] =='R')
	{
		if(tk->value[1] && tk->value[1] =='E')
		if(tk->value[2] && tk->value[2] =='T')
		if(tk->value[3] && tk->value[3] =='U')
		if(tk->value[4] && tk->value[4] =='R')
		if(tk->value[5] && tk->value[5] =='N')
		if(!tk->value[6])
			{
			clear(tk->value);
			tk->name = 	TK_RETURN;
			return 1;
			}
	}

	return 0;
}
