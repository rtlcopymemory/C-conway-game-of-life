SRC=main.c
SANITIZE=-fsanitize=address -fsanitize=leak
ARGS=-g -Wall -Werror
COMPILER=gcc

.PHONY: all
all: game-of-life

game-of-life: ${SRC}
	${COMPILER} ${ARGS} ${SRC} -o $@
