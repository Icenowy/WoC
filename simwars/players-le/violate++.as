LDA R0 %0x8001 # 0x8001 is for 0x8000+1, as PC is referenced in the next instruction
LTT PC R0
LDASU R1 %0x0000 # Interrupt position of Player 2
LDANS R1 %0xE000 # Interrupt position of Player 1
LDASU R2 %0x8000 # Code base of Player 2
LDANS R2 %0x4000 # Code base of Player 1
LDA R3 %0x0101 # Low 16 bits of INT %0 .
LDA R4 %16
LSH R3 R4
LDA R4 %0x0200 # High 16 bits
ADD R3 R4
STA R3 @R1 # Override the enemy's interrupt dealing program
LDA R4 %1
LDA R5 %3 # The loop is 3 instructions long
MOV R6 PC # Prepare to copy some code to interrupt procedure
LDA R7 %15 # 15 instructions to override loop, including this
LDASU R8 %0xE000 # Interrupt position of Player 1
LDANS R8 %0x0000 # Interrupt position of Player 2
ADD R6 R7
LDA R9 @R6 # Read from @R6, copy to @R8
STA R9 @R8
ADD R8 R4
ADD R6 R4
LDA R9 @R6
STA R9 @R8
ADD R8 R4
ADD R6 R4
LDA R9 @R6
STA R9 @R8
STA R3 @R2
ADD R2 R4
SUB PC R5
END . .
