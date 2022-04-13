CC=gcc
CCFLAGS=-Wall -g -O
LDFLAGS=
TARGET=attack

all: $(TARGET)

attack: attack.o mc48.o printhash.o
	$(CC) -o $@ $^ $(LDFLAGS) 

%.o: %.c %.h
	$(CC) $(CCFLAGS) -c $<

%.o: %.c
	$(CC) $(CCFLAGS) -c $<

clean:
	rm -f *.o $(TARGET)