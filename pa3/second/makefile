all : second

second : second.c
        gcc -g -Wall -Werror -std=c11 -fsanitize=address second.c -o second -lm

clean :
        rm -f second
