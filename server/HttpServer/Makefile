SRCS=main.cpp server.cpp json_converter.cpp
LDFLAGS=-levent
JANSONFLAG=-ljansson
PSTGR=-lpqxx -lpq
CXXFLAGS=-I/usr/local/include -L/usr/local/lib -L/usr/local/opt/libpq/lib -L/usr/local/opt/libpq/include
PROG=http_server

all:
	g++ -std=c++17 ${SRCS} -o ${PROG} ${LDFLAGS} ${JANSONFLAG} ${CXXFLAGS} ${PSTGR}

clean:
	rm -f ${PROG}