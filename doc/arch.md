# War of Co{d,r}es Architecture

## Memory address
65536 Addresses, each one contains 4 Bytes.

## Registers

- R0-R15: General Registers (ID 0x0-0xF)
- PC: Program Counter (ID 0x10) (Only the low 2 bytes are used) (pointer to next instruction)
- SP: Stack Pointer (ID 0x11) (Only the low 2 bytes are used) (pointer to last item)
- INT: Interrupt Reason (ID 0x12) (Only the low 1 byte is used)

## Interruptions

- 0x0: Invalid instruction

## Instructions

### General Statements

#### 000: (No Name) Always Execute
#### 001: IN Execute if in Interrupted mode
#### 010: SU Execure if last judgement instruction succeeded

### NOP: No operation

#### Grammar

NOP * * // the second and third segment is ignored

#### Binary Code

00000000 00000000 00000000 00000000 (The output of assembler)

JJJ00000 XXXXXXXX XXXXXXXX XXXXXXXX (Acceptable format)

JJJ00001 00000000 XXXXXXXX XXXXXXXX (Acceptable format 2)

JJJ00001 00000001 00000000 XXXXXXXX (Acceptable format 3)

### LDA: Load to a Register

#### Grammar

LDA REG %NUM // Let the value of REG be NUM (NUM must have at most 16 bits.)

LDA REG @REG // Access data at memory address REG

#### Binary Code

JJJ00010 RRRRRRRR NNNNNNNN NNNNNNNN // LDA REG %NUM

JJJ00001 00000010 RRRRRRRR RRRRRRRR // LDA REG @REG

### STA: Store from a Register

#### Grammar

STA REG @REG // Store data to memory address REG

#### Binary Code

JJJ00001 00000011 RRRRRRRR RRRRRRRR // STA REG @REG

### Arithmetic Instructions

#### Grammar

ADD REG1 REG2 // Add REG2 to REG1

SUB REG1 REG2 // Sub REG2 to REG1

#### Binary Code

JJJ00001 00000111 RRRRRRRR RRRRRRRR // ADD REG1 REG2

JJJ00001 00001000 RRRRRRRR RRRRRRRR // SUB REG1 REG2

### Judgement Instructions

#### Grammar

EQU REG1 REG2 // Success if REG1 == REG2

BGT REG1 REG2 // Success if REG1 > REG2

LTT REG1 REG2 // Success if REG1 < REG2

#### Binary Code

JJJ00001 00000100 RRRRRRRR RRRRRRRR // EQU REG1 REG2

JJJ00001 00000101 RRRRRRRR RRRRRRRR // BGT REG1 REG2

JJJ00001 00000110 RRRRRRRR RRRRRRRR // LTT REG1 REG2

### INT: Software Interrupt

#### Grammar

INT %NUM * // Interrupt with reason NUM (NUM must be at most 8 bits wide)

INT REG * // Interrupt with reason REG (only the low 1 byte is used)

#### Binary Code

JJJ00001 00000001 00000010 NNNNNNNN // INT NUM

JJJ00001 00000001 00000011 RRRRRRRR // INT REG

## Simple Wars Mode

The code of Player 0 will be placed randomly at 0x4000-0x7FFF, and the code of Player 1 will be placed randomly at 0x8000-0xBFFF.

Interrupt code for Player 0 is at 0xE000, and interrupt code for Player 1 is at 0x0000.

Initial SP for Player 0 is at 0x2000, and initial SP for Player 1 is at 0xC000.
