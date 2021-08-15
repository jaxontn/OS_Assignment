CC = gcc
CFLAGS = -Wall -ansi -pedantic -Werror -pthread -g
OBJ = multiThread.o pp.o srtf.o
EXEC = multiThread

$(EXEC) : $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) -pthread

multithread.o : multiThread.c multiThread.h pp.h srtf.h
	$(CC) $(CFLAGS) -c multithread.c

pp.o : pp.c pp.h multiThread.h
	$(CC) $(CFLAGS) -c pp.c

srtf.o : srtf.c srtf.h multiThread.h
	$(CC) $(CFLAGS) -c srtf.c

clean:
	rm -f $(EXEC) $(OBJ)
