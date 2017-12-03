CC=g++
CFLAGS=-I.
LFLAGS=-lrt -lpthread -lpigpiod_if2
DEPS = pir.hpp ledstrip.hpp ledstrip_controller.hpp
OBJ = pir.o ledstrip.o ledstrip_controller.o main.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ledctld: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LFLAGS)

.PHONY: clean

clean:
	rm -f *.o *~ core
