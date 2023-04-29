CC = gcc
CFLAGS = -g 
mydb: mydb.c buffer.c
	$(CC) $(CFLAGS) -o $@ $^ 
clean:
	rm -f mydb *.o

