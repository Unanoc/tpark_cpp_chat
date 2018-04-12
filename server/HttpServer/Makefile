SRCS=http_server.cpp
LDFLAGS=-levent
CXXFLAGS=-I/usr/local/include -L/usr/local/lib
PROG=http_server

all:
	g++ ${SRCS} -o ${PROG} ${LDFLAGS} ${CXXFLAGS}

clean:
	rm -f ${PROG}