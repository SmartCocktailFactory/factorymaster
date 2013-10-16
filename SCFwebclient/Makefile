
CC=g++
CFLAGS=-c -Wall -DDEBUG -g -ggdb

# Libraries
INCLUDE = -I/usr/local/include
LDFLAGS = -L/usr/local/lib 
LDLIBS = -lcurl -ljsoncpp -lstdc++

SOURCES = webclient.cpp com.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=webclient

.PHONY: all

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $@
#$(OUT): $(patsubst %.cpp,%.o,$(SOURCES))

#%.o: %.c
#	$(CC) -c  $(INCLUDE) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $<

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *o webclient
