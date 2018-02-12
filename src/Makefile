CC = gcc

CFLAGS = -Wall -ggdb

main: parser.tab.o lex.yy.o main.o tree.h tree.o pretty.h pretty.o
	$(CC) tree.o lex.yy.o parser.tab.o pretty.o main.o -o mini -lfl

parser.tab.c parser.tab.h: mini.y
	bison --yacc --defines=parser.tab.c --output=parser.tab.h mini.y

lex.yy.c: mini.l parser.tab.h
	flex mini.l

clean:
	rm parser.tab.* lex.yy.*; rm *.o; rm mini;
