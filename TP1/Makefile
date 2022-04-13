CC=gcc
CCFLAGS=-g -O
LDFLAGS=
SOURCE=mc48.o printhash.o
TARGET=attack run_hash

# Variable to define print type
ifeq ($(PRINTTYPE),1)
CCFLAGS+= -DPRINTCHAR
endif 

ifeq ($(PRINTTYPE),2)
CCFLAGS+= -DPRINTINT
endif

ifeq ($(PRINTTYPE),3)
CCFLAGS+= -DPRINT128
endif

# Variable to define verbose
ifeq ($(VERBOSE),1)
CCFLAGS+= -DVERBOSE
endif

all: $(TARGET)

attack: attack.o $(SOURCE)
	$(CC) -o $@ $^ $(LDFLAGS) 

run_hash: run_hash.o $(SOURCE)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c %.h
	$(CC) $(CCFLAGS) -c $<

%.o: %.c
	$(CC) $(CCFLAGS) -c $<

clean:
	rm -f *.o $(TARGET)