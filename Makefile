INCLUDE = ./include
INCLUDEPRIV = ./includepriv
CFLAGS = -O0 -g -Wall -Werror -pedantic -std=c99 -D_GNU_SOURCE -I$(INCLUDE) -I$(INCLUDEPRIV)

libserver.a: tcp_server.o udp_server.o server.o
	$(AR) rcs $@ $^

%.o: src/%.c
	$(CC) $< $(CFLAGS) -c -o $@

clean:
	rm *.o libserver.a
