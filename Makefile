CC = gcc
CFLAGS = -I./include
DEPS = navigate.h print.h write.h utils.h defaultDataPath.h
OBJ = src/main.o src/navigate.o src/print.o src/write.o src/utils.o

all: todo

%.o: %.c $(DEPS)
# $@ means the target file (e.g. "main.o")
# $< means the first prerequisite (e.g. "main.c")
	$(CC) -c -o $@ $< $(CFLAGS) 
	

todo: $(OBJ)
# $@ means the target file (e.g. "todo")
# $^ means all the prerequisites (e.g. "src/main.o")
	$(CC) -o $@ $^ $(CFLAGS)
	del src\*.o

clean:
	del todo.exe
	del src\*.o