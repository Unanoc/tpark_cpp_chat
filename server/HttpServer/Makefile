SRCS=main.cpp server.cpp
LDFLAGS=-levent
JANSONFLAG=-ljansson
CXXFLAGS=-I/usr/local/include -L/usr/local/lib
PROG=http_server

all:
	g++ ${SRCS} -o ${PROG} ${LDFLAGS} ${JANSONFLAG} ${CXXFLAGS}

clean:
	rm -f ${PROG}