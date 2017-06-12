#all: assembler emulator

emulator: emulator.o assembler.o program_counter.o bitmath.o
	gcc emulator.c -o emulator assembler.o program_counter.o bitmath.o -g
clean:
	rm emulator emulator.o bitmath.o program_counter.o assembler.o
