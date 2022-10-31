CC = gcc
CFLAGS = -g

randTest.out: randTest.c
	$(CC) $(CFLAGS) randTest.c -o randTest.out

clean:
	$(RM) randTest.out "numbers.txt"