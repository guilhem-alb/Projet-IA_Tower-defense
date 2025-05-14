CC = gcc
CFLAGS = -Wall -Wextra -Wmissing-prototypes -Wmissing-declarations -Wpointer-arith -Wreturn-type -Wcast-qual -fstack-protector-all
LIBS = -lraylib -lGL -lm -lpthread -ldl
SRCS = main.c maze.c astar.c ballon.c tower.c projectile.c listeCases.c
OBJS = $(SRCS:.c=.o)
EXEC = balloon_maze_defense

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean