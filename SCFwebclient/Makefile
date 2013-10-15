
CC=g++
CFLAGS=-c -Wall

# Libraries
INCLUDE = -I/usr/local/include
LDFLAGS = -L/usr/local/lib 
LDLIBS = -lcurl

SOURCES = webclien.cpp com.cpp
OUT = webclient

.PHONY: all

all: webclient

$(OUT): $(patsubst %.cpp,%.o,$(SOURCES))

%.o: %.c
	$(CC) -c $(INCLUDE) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $<

clean:
	rm -rf *o webclient
