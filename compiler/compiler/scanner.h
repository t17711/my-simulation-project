#pragma once
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
	map<string, token_name> key_table;
	int key_n;
	bool proceed = false;
	bool error = false;

public:
	scanner();
	~scanner();
	void print();
	void get_eof();
	void get_key_table();
	void check_keyword(token* tk);
	void get_token();
	void identifier();
	void comment(int type);
	void get_char_token();
	void digit_token();
	void get_string_token();
	void operator_token();
	void float_token();
	void exp_token();
};