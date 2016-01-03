# infector.s
# Copyright (C) StarBrilliant <m13253@hotmail.com>
# Released under GPL license version 3

mov	r15	pc
lda	r2	%12
add	r15	r2

# Const 0
lda	r0	%0
# Const 1
lda	r1	%1
# Const 8
lda	r8	%8
# Const 9
lda	r9	%9
# Const 10
lda	r10	%16
# Addr 11
lda	r11	%0x8000
ltt	r15	r11
ldans	r11	%0x4000
# Const 12
lda	r12	%0xe000
# Const 13
sub	r13	r1

# Protect myself 
# Label 1
lda	r2	%0x010b
lsh	r2	r10
lda	r3	%0x100f
add	r2	r3
stasu	r2	@r12
stans	r2	@r0

# Attack enemy
stasu	r13	@r0
stans	r13	@r12

# Fork
lda	r14	%0x1100
add	r14	r15
rsh	r14	r8
lsh	r14	r8
add	r14	r8
mov	r3	r14
mov	r4	r15
lda	r5	%54
lda	r6	%7

# Label 2
lda	r2	@r4
add	r4	r1
sta	r2	@r3
add	r3	r1
sub	r5	r1

# Goto label 2
equ	r5	r0
subns	pc	r6

# Infect
lda	r3	%2
lda	r5	%58
lda	r6	%14
lda	r7	%7

# Label 3
equ	r11	r0
addsu	pc	r1
equ	r11	r12
addsu	r11	r1

lda	r2	@r11
# Goto label 4
equ	r2	r0
addsu	pc	r3
equ	r2	r13
addns	pc	r7

sta	r13	@r11
add	r11	r9
sub	r5	r1

# Goto label 3
equ	r5	r0
subns	pc	r6

# Goto label 1
mov	r15	r14
mov	pc	r15

# Label 4
lda	r5	%65
lda	r6	%5

# Label 5
sta	r13	@r11
add	r11	r1
sub	r5	r1

# Goto label 5
equ	r5	r0
subns	pc	r6

# Goto label 1
mov	r15	r14
mov	pc	r15

end	.	.
