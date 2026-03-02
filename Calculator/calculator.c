#define _CRT_SECURE_NO_WARNINGS

#include "calculator.h"

struct Stack* stack;


// Token

struct Token* token_create(char kind) {
	struct Token* token;

	token = (struct Token*)malloc(sizeof(struct Token*));
	if (token == NULL) {
		fprintf(stderr, "token_create: allocating memory for token failed!\n");
		return NULL;
	}

	token->kind = kind;
	token->value = 0;

	return token;
}

int token_set_value(struct Token* token, double value) {
	if (token == NULL) {
		fprintf(stderr, "token_set_value: token is NULL\n");
		return -1;
	}

	token->value = value;
	return 0;
}

int token_free(struct Token** token) {
	free((*token));
	return 0;
}

// Stack


struct Stack* stack_create(int capacity) {
	struct Stack* stack;

	stack = (struct Stack*)malloc(sizeof(struct Stack));
	if (stack == NULL) {
		fprintf(stderr, "stack_create: allocating memory for stack failed!\n");
		return NULL;
	}

	stack->tokens = (struct Token*)malloc(sizeof(struct Token) * capacity);
	if (stack->tokens == NULL) {
		fprintf(stderr, "stack_create: allocating memory for stack tokens failed\n");
		return NULL;
	}

	stack->capacity = capacity;
	stack->length = 0;

	return stack;
}

struct Token* stack_pop(struct Stack* stack) {
	struct Token* token;

	if (stack == NULL || stack->tokens == NULL) {
		fprintf(stderr, "stack_pop: stack is NULL\n");
		return NULL;
	}

	stack->length = stack->length - 1;
	token = stack->tokens[stack->length];
	stack->tokens[stack->length] = NULL;

	//printf("popped token is %c\n", token->kind);

	return token;
}

int stack_push(struct Stack* stack, struct Token* token) {

	if (stack == NULL || stack->tokens == NULL) {
		fprintf(stderr, "stack_push: stack is NULL\n");
		return -1;
	}

	if (token == NULL) {
		fprintf(stderr, "stack_push: token is NULL\n");
		return -1;
	}

	if (stack_isfull(stack)) {
		if (stack_extend(&stack) != 0) {
			return -1;
		}
	}

	stack->tokens[stack->length] = token;
	stack->length++;
	return 0;
}

int stack_size(struct Stack* stack) {
	if (stack == NULL || stack->tokens == NULL) {
		fprintf(stderr, "stack_size: stack is NULL\n");
		return -1;
	}

	return stack->length;
}

int stack_isempty(struct Stack* stack) {
	if (stack == NULL || stack->tokens == NULL) {
		fprintf(stderr, "stack_isempty: stack is NULL\n");
		return -1;
	}
	//printf("stack->length = %d\n", stack->length);

	if (stack->length == 0) {
		return 1;
	}

	return 0;
}

int stack_isfull(struct Stack* stack) {
	if (stack == NULL || stack->tokens == NULL) {
		fprintf(stderr, "stack_isfull: stack is NULL\n");
		return -1;
	}

	if (stack->length == (stack->capacity + 1)) {
		return 1;
	}

	return 0;
}


int stack_extend(struct Stack* stack) {
	struct Tokens** tokens;
	int i;

	if (stack == NULL || stack->tokens == NULL) {
		fprintf(stderr, "stack_extend: stack is NULL\n");
		return -1;
	}

	tokens = realloc(stack->tokens, stack->capacity * 2);
	if (stack->tokens == NULL) {
		fprintf(stderr, "stack_extend: extending tokens array failed\n");
		return -1;
	}

	for (i = 0; i < stack->length; i++) {
		token_free(&stack->tokens[i]);
	}

	stack->tokens = tokens;
	stack->capacity = stack->capacity * 2;

	return 0;
}

int stack_free(struct Stack** stack) {
	int i;

	if (*stack == NULL || (*stack)->tokens == NULL) {
		fprintf(stderr, "stack_free: stack is NULL\n");
		return -1;
	}


	for (i = 0; i < (*stack)->length; i++) {
		if (token_free(&(*stack)->tokens[i]) != 0) {
			return -1;
		}
	}

	free(*stack);

	return 0;
}


/* Calculator */

void calculator_init() {

	if (stack == NULL) {
		stack = stack_create(10);
		if (stack == NULL) {
			fprintf(stderr, "calculator_init: allocating memory for stack failed!\n");
			return;
		}
	}
	else {
		fprintf(stderr, "info: stack is not NULL\n");
		return;
	}

}

void calculator_deinit() {
	int i;

	if (stack == NULL) {
		fprintf(stderr, "calculator_deinit: stack is NULL\n");
		return;
	}

	for (i = 0; i < stack->length; i++) {
		if (token_free(&stack->tokens[i]) != 0) {
			fprintf(stderr, "calculator_deinit: deallocating token failed!\n");
			return;
		}
	}

	if (stack_free(&stack) != 0) {
		fprintf(stderr, "calculator_deinit: deallocating stack failed!\n");
		return;
	}
	

}

struct Token* calculator_get_token() {
	struct Token* token = NULL;
	char* input;
	double number;
	char ch;
	int i;

	if (stack == NULL) {
		fprintf(stderr, "calculator_get_token: stack is not initialized!\n");
		exit(1);
	}

	if (!stack_isempty(stack)) {
		//printf("stack is not empty!\n");
		return stack_pop(stack);
	}

	if (fscanf(stdin, " %c", &ch) != 1) {
		fprintf(stderr, "get_token: an error occurred while taking input!\n");
		calculator_deinit();
		exit(1);
	}

	//printf("ch is %c\n", ch);
	switch (ch) {
	case '+':
	case '-':
	case '*':
	case '/':
	case ';':
	case '(':
	case ')':
	case 'q':
		//printf("operator %c\n", ch);
		token = token_create(ch);

		break;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':

		ungetc(ch, stdin);
		input = string_create(INPUT_SIZE);
		if (input == NULL) {
			fprintf(stderr, "calcualtor_get_token: allocating memory for input string failed!\n");
			calculator_deinit();
			exit(1);
		}

		i = 0;
		while (isdigit(ch = fgetc(stdin)) || ch == '.') {
			input[i] = ch;
			i++;
		}
		ungetc(ch, stdin); /* Putting the last character back in */

		number = atof(input);

		string_free(&input);

		token = token_create('8');
		token_set_value(token, number);
		
		break;


	default:
		calculator_deinit();
		fprintf(stderr, "get_token: invalid input: %c\n", ch);
		exit(1);
	}


	return token;
}

int calculator_unget_token(struct Token* token) {
	if (token == NULL) {
		fprintf(stderr, "calcualtor_unget_token: token is nULL");
		return -1;
	}

	if (stack_push(stack, token) != 0) {
		return -1;
	}

	return 0;
}

double calculator_expression() {
	double left = calculator_term();
	struct Token* token = calculator_get_token();

	while (1) {
		switch (token->kind) {
		case '+':
			left += calculator_term();
			//token_free(&token);
			token = calculator_get_token();
			break;
		case '-':
			left -= calculator_term();
			//token_free(&token);
			token = calculator_get_token();
			break;
		default:
			stack_push(stack, token);
			return left;
		}
	}

}

double calculator_term() {
	double left = calculator_primary();
	double value;
	struct Token* token = calculator_get_token();

	while (1) {
		switch (token->kind) {
		case '*':
			left *= calculator_primary();
			//token_free(&token);
			token = calculator_get_token();
			break;
		case '/':
			value = calculator_primary();
			if (value == 0) {
				fprintf(stderr, "error: division by zero");
				return left;
			}
			left = left / value;
			//token_free(&token);
			token = calculator_get_token();
			break;
		default:
			stack_push(stack, token);
			return left;
		}
	}
}

double calculator_primary() {
	double value = 0;
	struct Token* token = calculator_get_token();

	//printf("got this %c\n", token->kind);

	switch (token->kind) {
	case '(':
		value = calculator_expression();
		//token_free(&token);
		token = calculator_get_token();
		if (token->kind != ')') {
			fprintf(stderr, "error: missing closing parenthesis ')'!\n");
		}
		//token_free(&token);

		break;
	case '8':
		value = token->value;
		break;
	case '-':
		value = -calculator_primary();
		break;
	case '+':
		value = calculator_primary();
		break;
	default:
		printf(stderr, "error: invalid expression!\n");
		calculator_deinit();
		exit(1);
	}

	return value;
}

void calculator_controller() {
	struct Token* token;
	double value = 0;
	int should_quit = 0;

	calculator_init();

	while (1) {
		printf("> ");
		token = calculator_get_token();
		while (token->kind == ';') {
			//token_free(&token);
			token = calculator_get_token();
		}
		if (token->kind == 'q') {
			//token_free(&token);
			break;
		}

		stack_push(stack, token);
		printf("= %f\n", calculator_expression());
	}

	calculator_deinit();
}


/* Helper Utils */


char* string_create(int size) {
	char* string;

	string = malloc(sizeof(char) * size);
	if (string == NULL) {
		calculator_deinit();
		exit(1);
	}

	return string;
}

void string_free(char** string) {
	free(*string);
}

int string_clean(char* string, int size) {
	int i;

	if (string == NULL) {
		fprintf(stderr, "string_clean: string is nULL\n");
		return -1;
	}

	for (i = 0; i < size; i++) {
		string[i] = 0;
	}

	return 0;
}
