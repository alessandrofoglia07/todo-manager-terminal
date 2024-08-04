CC = gcc
CFLAGS = -I./include
LDFLAGS = -lreadline
DEPS = navigate.h print.h write.h utils.h ANSIEscapeCodes.h globals.h
OBJ = src/main.o src/navigate.o src/print.o src/write.o src/utils.o src/globals.c

all: todo

%.o: %.c $(DEPS)
# $@ means the target file (e.g. "main.o")
# $< means the first prerequisite (e.g. "main.c")
	$(CC) -c -o $@ $< $(CFLAGS) 
	

todo: $(OBJ)
# $@ means the target file (e.g. "todo")
# $^ means all the prerequisites (e.g. "src/main.o")
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
	del src\*.o

clean:
	del todo.exe
	del src\*.o