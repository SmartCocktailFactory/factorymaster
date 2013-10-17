CC=g++
CFLAGS=-c -Wall

# Libraries
INCLUDE = -I/usr/local/include
LDFLAGS = -L/usr/local/lib
LDLIBS = -lpthread -lcurl -ljsoncpp

SOURCES = main.cpp HueCom.cpp

OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=hue

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf src/*o hue
