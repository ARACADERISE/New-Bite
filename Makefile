NORMAL_C = $(wildcard src/*.c)
LEXER_C = $(wildcard src/lexer/*.c)
KEYWORD_LEXER_C = $(wildcard src/lexer/keyword_lexer/*.c)
PARSER_C = $(wildcard src/parser/*.c)
SYNTAX_TREE_C = $(wildcard src/SyntaxTree/*.c)
COMPILER_C = $(wildcard src/compiler/*.c)
COMPILE = gcc -Wall -o main.o main.c
COMPILE_ = gcc -S main.c
COMPILE_ALL_S = nasm -f elf *.s
# ld -m elf_i386 -s -o demo cmpler.o

FILE = examples/main.b

.PHONY: RunBite
.PHONY: ClearBite
.PHONY: all

# nasm -f elf *.s; ld -m elf_i386 -s -o demo cmpler.o
#as cmpler.s -o out.o --32 && ld -m elf_i386 -s -o out out.o && ./out
#${COMPILE} ${NORMAL_C} ${LEXER_C} ${KEYWORD_LEXER_C} ${PARSER_C} ${PARSER_H} ${SYNTAX_TREE_C} ${COMPILER_C} && ./main.o examples/main.b && nasm -f elf *.s; ld -m elf_i386 -s -o demo cmpler.o && ./demo

RunBite:
	@${COMPILE} ${NORMAL_C} ${LEXER_C} ${KEYWORD_LEXER_C} ${PARSER_C} ${PARSER_H} ${SYNTAX_TREE_C} ${COMPILER_C} && ./main.o $(FILE) && nasm -f elf *.s; ld -m elf_i386 -s -o demo cmpler.o && ./demo


ClearBite:
	@rm *.o