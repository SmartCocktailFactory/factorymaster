CC=g++
CFLAGS=-c -Wall

# Libraries
INCLUDE = -I/usr/local/include
LDFLAGS = -L/usr/local/lib
LDLIBS = -lpthread -lcurl -ljsoncpp

SOURCES = src/main.cpp src/DrinkController.cpp src/BottleManagement.cpp src/LiquidDeliverySystem.cpp src/TCP.cpp src/com.cpp

OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=SCFDrinkController

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf src/*o SCFDrinkController
