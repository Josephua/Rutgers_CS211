all: second

second : second.c
        clang -g -Wall -Werror -fsanitize=address second.c -o second

clean:
        rm second
