run: scanner.c parser.c
	gcc -std=c99 -o compiler parser.c 