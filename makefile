#all: assembler emulator

#assembler:  assembler.o
#	gcc assembler.c -o assembler

emulator: emulator.o
	gcc emulator.c -o emulator assembler.h program_counter.h bitmath.h
clean:
	rm emulator assembler
