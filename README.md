# IBM701Emulator
## Created By Jason Gallagher

CS T480 - History of Computing

This emulator simulates an IBM 701, originally known as the Defense Calculator. The IBM 701 was IBM's first
commercial scientific computer. This computer was announced on April 29, 1952 and its main purpose was to aid
in the development of military aircrafts to be used in the korean war.
For more information see: https://en.wikipedia.org/wiki/IBM_701
### Emulator commands
Command Description
quit    Exits the emulator

ppc     Prints program counter details

sv      Set value, must hit enter before entring value

ex      Execute instruction at current memory address
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

### Instructions:

0000 STOP	Stop and Transfer        Halt; restart address = addr

00001 TR        Transfer                 Jump to (addr)

00010 TROF     Transfer on Overflow     if overflow, jump to (addr)

00011 TR+      Transfer on Plus         if ACC greater than or equal to 0, jump to (addr)

00100 TR0      Transfer on Zero         if ACC = 0, jump to (addr)

00101 SUB       Subtract                 ACC = ACC - MEM(addr)

00110 RSUB     Reset and Subtract       ACC = 0 - MEM(addr)

00111 SUBAB    Subtract Absolute Value  ACC = ACC - ABS(MEM(addr))

01001 ADD       Add                      ACC = ACC + MEM(addr)

01010 RADD     Reset and Add            ACC = MEM(addr)

01011 ADDAB    Add Absolute Value       ACC = ACC + ABS(MEM(addr))

01100 STORE     Store                    MEM(addr) = ACC

001101 STOREA   Store Address            MEM(addr)[6..17] = ACC[6..17]

101101 EXTR      Extract                  MEM(addr) = MEM(addr) AND ACC

01110 STOREMQ  Store MQ                 MEM(addr) = MQ

01111 LOADMQ   Load MQ                  MQ = MEM(addr)

10000 MPY       Multiply                 [ACC,MQ] = MQ * MEM(addr)

10001 MPYROUND Multiply and Round       MPY then ROUND

10010 DIV       Divide                   MQ = [ACC,MQ]/MEM(addr); ACC = remainder

10011 ROUND     Round                    if MQ high bit set, ACC = ACC + 1

10100 LLEFT    Long Left Shift          shift [ACC,MQ] left addr places

10101 LRIGHT   Long Right Shift         shift [ACC,MQ] right addr places

10110 ALEFT    Accumulator Left Shift   shift ACC left addr places

10111 ARIGHT   Accumulator Right Shift  shift ACC right addr places
