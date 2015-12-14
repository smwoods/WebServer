all: server my-histogram

server:
	gcc -Wall server.c -o server

my-histogram:
	gcc -Wall my-histogram.c -o my-histogram

clean:
	rm server && rm my-histogram

nogger:
	gcc -Wall nogger.c -o nogger

cnogger:
	rm nogger