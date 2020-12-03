all : third

third : third.c
	gcc -g -Wall -Werror -std=c11 -fsanitize=address third.c -o third -lm

clean :
	rm -f third