LDA R0 %0x8001
LTT PC R0
LDASU R1 %0
LDANS R1 %0xE000
LDASU R2 %0x8000
LDANS R2 %0x4000
LDA R3 %0x0101
LDA R4 %16
LSH R3 R4
LDA R4 %0x0200
ADD R3 R4
STA R3 @R1
LDA R4 %1
LDA R5 %3
STA R3 @R2
ADD R2 R4
SUB PC R5
END . .
