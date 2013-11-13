CCFLAGS=

all: server client

server: server.c
	gcc $(CCFLAGS) -o server server.c

client: client.c
	gcc $(CCFLAGS) -o client client.c

clean:
	rm -r client server *.dSYM
