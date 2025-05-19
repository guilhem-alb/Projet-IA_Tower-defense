CC = gcc
CFLAGS = -Wall -Wextra -Wpointer-arith -Wreturn-type -Wcast-qual -fstack-protector-all
LIBS = -lraylib -lGL -lm -lpthread -ldl
SRCS = main.c maze.c astar.c ballon.c tower.c projectile.c listeCases.c
OBJS = $(SRCS:.c=.o)
EXEC = bloons_maze_battle

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean