# IBM701Emulator
## Created By Jason Gallagher

CS T480 - History of Computing

This emulator simulates an IBM 701, originally known as the Defense Calculator. The IBM 701 was IBM's first
commercial scientific computer. This computer was announced on April 29, 1952 and its main purpose was to aid
in the development of military aircrafts to be used in the korean war.
For more information see: https://en.wikipedia.org/wiki/IBM_701
### Emulator commands
Command Description
Help menu ------------

ppc - print program pointer details

sv - save next binary input into current memory location

ex - executes instruction in current register

help - show this menu

pm - print memory contents from -10 to +10 address's around the current address

op - prints a list of the accepted opcodes. To enter an opcode just type the opcode and the v

file - opens up a file, for relative directory type ./file_name

quit - exits the emulator

save - save contents of the memory registers

jump # - this command followed by a number will jump the program counter to that memory address

run - executes 10 instructions displaying the register contents each execution


### Architecture:
#### Memory:
	
    *72 williams cathode tubes, 1024 bits each
		
        *2048 words
		
        *36 bit words

#### Instructions:
	
    *18 bits long
		
        * 1 sign bit
		
        * 5 bit opcode
		
        * 12 bit address

#### Program Accessible Registers:
	
    * accumulator: 38 bit
	
    * multiplier/quotient: 36 bits

### Program commands:
Help menu ------------

ppc - print program pointer details

sv - save next binary input into current memory location

ex - executes instruction in current register

help - show this menu

pm - print memory contents from -10 to +10 address's around the current address

op - prints a list of the accepted opcodes. To enter an opcode just type the opcode and the value of the instruction

file - opens up a file, for relative directory type ./file_name


### Instructions:

0000 STOP	Stop and Transfer        Halt; restart address = addr

00001 TR        Transfer                 Jump to (addr)

00010 TR OF     Transfer on Overflow     if overflow, jump to (addr)

00011 TR +      Transfer on Plus         if ACC greater than or equal to 0, jump to (addr)

00100 TR 0      Transfer on Zero         if ACC = 0, jump to (addr)

00101 SUB       Subtract                 ACC = ACC - MEM(addr)

00110 R SUB     Reset and Subtract       ACC = 0 - MEM(addr)

00111 SUB AB    Subtract Absolute Value  ACC = ACC - ABS(MEM(addr))

01001 ADD       Add                      ACC = ACC + MEM(addr)

01010 R ADD     Reset and Add            ACC = MEM(addr)

01011 ADD AB    Add Absolute Value       ACC = ACC + ABS(MEM(addr))

01100 STORE     Store                    MEM(addr) = ACC

001101 STORE A   Store Address            MEM(addr)[6..17] = ACC[6..17]

101101 EXTR      Extract                  MEM(addr) = MEM(addr) AND ACC

01110 STORE MQ  Store MQ                 MEM(addr) = MQ

01111 LOAD MQ   Load MQ                  MQ = MEM(addr)

10000 MPY       Multiply                 [ACC,MQ] = MQ * MEM(addr)

10001 MPY ROUND Multiply and Round       MPY then ROUND

10010 DIV       Divide                   MQ = [ACC,MQ]/MEM(addr); ACC = remainder

10011 ROUND     Round                    if MQ high bit set, ACC = ACC + 1

10100 L LEFT    Long Left Shift          shift [ACC,MQ] left addr places

10101 L RIGHT   Long Right Shift         shift [ACC,MQ] right addr places

10110 A LEFT    Accumulator Left Shift   shift ACC left addr places

10111 A RIGHT   Accumulator Right Shift  shift ACC right addr places

##Notes:
I haven't been able to thoroughly.
For the save command you must type binary, but to enter in an assembly command you just type

Assembly key integervalue

To import a file, type file enter, then type in the path to your file starting with ./ if 
it is a local path.

#Binary Input:
_______________________________________
opcode|address     |opcode|address     |
101010 101010101010 101010 101010101010|
---------------------------------------|

Accumulator
__________________________________________
S|Q|P|value                               |
0 0 1 101010101010101010101010101010101010|
------------------------------------------|

Multpilier_quotient:
______________________________________
S|Value                               |
1 101010101010101010101010101010101010|
--------------------------------------|

* currently the emulator only supports full word addressing, the defense calculator
could be toggled from 18 bit words to 36 bit words depending on the use case. This
just means each instruction has its own 36 bit memory space.


##Demo:
sample.instr - I have provided a sample application which is the source code for the applying
the difference engine method, using the equation from our eniac homework.

###To import:
file
./sample.instr

To make sure the file was imported correctly type ppc. This displays the console details
you would have on the actual machine.

To start the application, copy the first instruction that was imported. This is a transfer
instruction that will tak you to the start of the application.you can view the 10 previous
and 10 following lines of assembly by running the command pm.

###Commands:
sv
100000011000000000000000000000000000
ex

At this point, you simply repeatedly hit ex to execute, and ppc to view the contents of
the registers.


