NORMAL_C = $(wildcard src/*.c)
LEXER_C = $(wildcard src/lexer/*.c)
KEYWORD_LEXER_C = $(wildcard src/lexer/keyword_lexer/*.c)
PARSER_C = $(wildcard src/parser/*.c)
SYNTAX_TREE_C = $(wildcard src/SyntaxTree/*.c)
COMPILER_C = $(wildcard src/compiler/*.c)
COMPILE = gcc -Wall -o main.o main.c

.PHONY: RunBite
.PHONY: ClearBite

# nasm -f elf *.s; ld -m elf_i386 -s -o demo cmpler.o
#as cmpler.s -o out.o --32 && ld -m elf_i386 -s -o out out.o && ./out

RunBite:
	${COMPILE} ${NORMAL_C} ${LEXER_C} ${KEYWORD_LEXER_C} ${PARSER_C} ${PARSER_H} ${SYNTAX_TREE_C} ${COMPILER_C} && ./main.o examples/main.b && nasm -f elf *.s; ld -m elf_i386 -s -o demo cmpler.o && ./demo


ClearBite:
	rm *.o
	rm *.s