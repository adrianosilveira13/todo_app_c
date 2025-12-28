CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -O2
INC=-Isrc

SRC = \
  src/main.c \
  src/domain/task.c \
  src/app/todo_app.c \
  src/util/text.c \
  src/infra/task_repo_sqlite.c \
  src/ui/cli/cli.c

all: todo

todo: $(SRC)
	$(CC) $(CFLAGS) $(INC) $(SRC) -o todo -lsqlite3

clean:
	rm -f todo