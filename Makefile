NORMAL_C = $(wildcard src/*.c)
LEXER_C = $(wildcard src/lexer/*.c)
KEYWORD_LEXER_C = $(wildcard src/lexer/keyword_lexer/*.c)
PARSER_C = $(wildcard src/parser/*.c)
SYNTAX_TREE_C = $(wildcard src/SyntaxTree/*.c)
COMPILE = gcc -Wall -o main.o main.c

.PHONY: RunBite
.PHONY: ClearBite

RunBite:
	${COMPILE} ${NORMAL_C} ${LEXER_C} ${KEYWORD_LEXER_C} ${PARSER_C} ${PARSER_H} ${SYNTAX_TREE_C} && ./main.o

ClearBite:
	rm *.o