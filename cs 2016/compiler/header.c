
unsigned get_token		(char *buffer, int i, struct token* token_list , int j);

unsigned get_eof		(struct token* token_list, int j);
unsigned comment		(char *buffer,int i, struct token* token_list, int j, int type);
unsigned identifier		(char *buffer,int i, struct token* token_list, int j);
unsigned digit_token	(char *buffer,int i, struct token* token_list, int j);
unsigned double_token	(char *buffer,int i, struct token* token_list, int j);
unsigned exp_token		(char *buffer,int i, struct token* token_list, int j);

unsigned get_string_token(char *buffer,int i, struct token* token_list, int j);

unsigned operator_token	(char *buffer,int i, struct token* token_list, int j);
unsigned include_token	(char *buffer,int i, struct token* token_list, int j, int type);
//unsigned define_token	(char *buffer,int i, struct token* token_list, int j);
unsigned hash_token	(char *buffer,int i, struct token* token_list, int j);
