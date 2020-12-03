all : fourth

fourth : fourth.c
        gcc -g -Wall -Werror -std=c11 -fsanitize=address fourth.c -o fourth

clean :
        rm -f fourth
