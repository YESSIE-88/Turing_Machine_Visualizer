CC = gcc
CFLAGS = -Wall -g
TARGET = turing_machine_visualizer

all: $(TARGET)

$(TARGET): main.o dll.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o dll.o

main.o: main.c dll.h
	$(CC) $(CFLAGS) -c main.c

dll.o: dll.c dll.h
	$(CC) $(CFLAGS) -c dll.c

clean:
	rm -f *.o $(TARGET)