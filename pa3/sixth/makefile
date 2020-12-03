all : sixth

sixth : sixth.c
	gcc -g -Wall -Werror -std=c11 -fsanitize=address sixth.c -o sixth -lm

clean :
	rm -f sixth