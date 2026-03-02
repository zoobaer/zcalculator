#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INPUT_SIZE 100

struct Token {
	char kind;
	double value;
};

struct Token* token_create(char kind);
int token_set_value(struct Token* token, double value);
int token_free(struct Token** token);

struct Stack {
	struct Token** tokens;
	int length;
	int capacity;
};

struct Stack* stack_create(int capacity);
struct Token* stack_pop(struct Stack* stack);
int stack_extend(struct Stack* stack);
int stack_free(struct Stack** stack);

/* Calculator */

void calculator_init();
void calculator_deinit();
struct Token* calculator_get_token();
int calculator_unget_token(struct Token* token);
double calculator_expression();
double calculator_term();
double calculator_primary();
void calculator_controller();

/* Helper utils */

char* string_create(int size);
void string_free(char** string);
int string_clean(char* string, int size);


#endif