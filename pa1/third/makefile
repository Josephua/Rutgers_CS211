all: third

second : third.c
	clang -g -Wall -Werror -fsanitize=address third.c -o third

clean:
	rm third