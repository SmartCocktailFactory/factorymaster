
CC=g++
CFLAGS=-c -Wall -DDEBUG -g -ggdb

# Libraries
INCLUDE = -I/usr/local/include
LDFLAGS = -L/usr/local/lib 
LDLIBS = -lcurl -ljsoncpp -lstdc++

SOURCES = webclien.cpp com.cpp
OUT = webclient

.PHONY: all

all: webclient

$(OUT): $(patsubst %.cpp,%.o,$(SOURCES))

%.o: %.c
	$(CC) -c  $(INCLUDE) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $<

clean:
	rm -rf *o webclient
