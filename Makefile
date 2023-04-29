CC = gcc
CFLAGS = -g
src = $(wildcard *.c)
mydb: $(src)
	$(CC) $(CFLAGS) -o $@ $^ 
clean:
	rm -f mydb *.o

