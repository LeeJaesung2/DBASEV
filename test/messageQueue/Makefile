CC=g++
CFLAGS=-c -Wall
LDFLAGS=-lpthread -ltbb -std=c++14
PYFLAGS = -I/usr/include/python3.8 -lpython3.8

EXECUTABLE= main.exe

all: $(EXECUTABLE)

$(EXECUTABLE) : main.o messageQueue.o
	$(CC) main.o messageQueue.o -o $(EXECUTABLE) $(LDFLAGS) $(PYFLAGS)

messageQueue.o : messageQueue.cpp
	$(CC) $(CFLAGS) $(PYFLAGS) messageQueue.cpp -o messageQueue.o

main.o : main.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) main.cpp -o main.o

clean:
	rm messageQueue.o main.o main.exe consumer.pyc