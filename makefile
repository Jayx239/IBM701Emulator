CC=gcc
CFLAGS=-std=c99 -g

emulator: assembler.o program_counter.o bitmath.o emulator.o
	$(CC) $(CFLAGS) emulator.c -o emulator assembler.o program_counter.o bitmath.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
clean:
	rm emulator emulator.o bitmath.o program_counter.o assembler.o
