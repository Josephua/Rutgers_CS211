all : fifth

fifth : fifth.c
        gcc -g -Wall -Werror -std=c11 -fsanitize=address fifth.c -o fifth -lm

clean :
        rm -f fifth
