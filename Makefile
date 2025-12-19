CC = gcc
CFLAGS = -Wall -g
TARGET = turing_machine_visualizer

# Object files
OBJS = main.o dll.o tm_parser.o turing_machine.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c dll.h tm_parser.h turing_machine.h
	$(CC) $(CFLAGS) -c main.c

dll.o: dll.c dll.h
	$(CC) $(CFLAGS) -c dll.c

tm_parser.o: tm_parser.c tm_parser.h
	$(CC) $(CFLAGS) -c tm_parser.c

turing_machine.o: turing_machine.c turing_machine.h dll.h
	$(CC) $(CFLAGS) -c turing_machine.c

clean:
	rm -f *.o $(TARGET)