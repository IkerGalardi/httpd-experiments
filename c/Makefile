CC=cc

CONFIG=debug

CFLAGS=-Wall -Wextra -Wpedantic -std=c17 -Isrc

OBJS=bin/main.o    \
     bin/config.o  \

all: httpd

httpd: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o httpd

bin/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm httpd
	rm bin/*.o