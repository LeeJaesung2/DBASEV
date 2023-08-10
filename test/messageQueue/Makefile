CC=g++
CFLAGS=-c -Wall
LDFLAGS=-lpthread -std=c++14
PYTHON_VERSION := $(shell python -c "import sys; print('.'.join(map(str, sys.version_info[:2])))")
PYFLAGS = -I/usr/include/python$(PYTHON_VERSION) -lpython$(PYTHON_VERSION)

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