CC = gcc

CFLAGS = -Wall

main: parser.tab.o lex.yy.o main.o tree.h tree.o pretty.h pretty.o
	$(CC) lex.yy.o parser.tab.o tree.o pretty.o main.o -o mini -lfl

parser.tab.c parser.tab.h: mini.y
	bison --yacc --defines=parser.tab.c --output=parser.tab.h mini.y

lex.yy.c: mini.l parser.tab.h
	flex mini.l

clean:
	rm parser.tab.* lex.yy.*; rm *.o; rm mini;
