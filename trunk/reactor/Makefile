FLAGS  := -g -Wall
CC     := g++
LIB    := libreactor.a

all: server client
server:
	$(CC) $(FLAGS) reactor.cc event_demultiplexer.cc reactor_server_test.cc -o $@
client:
	$(CC) $(FLAGS) reactor.cc event_demultiplexer.cc reactor_client_test.cc -o $@
clean:
	rm server 2>/dev/null; rm client 2>/dev/null
