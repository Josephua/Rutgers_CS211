all : fourth

fourth : fourth.c
	gcc -g -Wall -Werror -std=c11 -fsanitize=address fourth.c -o fourth -lm

clean :
	rm -f fourth