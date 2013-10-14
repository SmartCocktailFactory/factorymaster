# I am a comment, and I want to say that the variable CC will be
# the compiler to use.
CC=g++
# Hey!, I am comment number 2. I want to say that CFLAGS will be the
# options I'll pass to the compiler.
CFLAGS=-c -Wall



all: webclient

webclient: webclient.o com.o
	$(CC) webclient.o com.o -o webclient

webclient.o: webclient.cpp
	$(CC) $(CFLAGS) webclient.cpp

com.o: com.cpp
	$(CC) $(CFLAGS) com.cpp

clean:
	rm -rf *o webclient
