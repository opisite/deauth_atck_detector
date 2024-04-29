# Define the compiler
CC=g++

# Define any compile-time flags
CFLAGS=-Wall -g

# Define any libraries to link into executable:
LIBS=-lpthread -lpcap

# Define the C source files
SRCS=$(wildcard src/*.cpp)

# Define the C object files
OBJS=$(SRCS:.c=.o)

# Define the executable file
MAIN=deauth-tester

.PHONY: depend clean

all:    $(MAIN)
	@echo  Compiling $(MAIN) completed.

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS) $(LIBS)

# This is a suffix replacement rule for building .o's from .c's
.c.o:
	$(CC) $(CFLAGS) -c $<  -o $@

clean:
	rm -f *.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $^
