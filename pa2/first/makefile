all : first

first : first.c
	gcc -g -Wall -Werror -std=c11 -fsanitize=address first.c -o first

clean :
	rm -f first
