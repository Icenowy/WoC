#
infector.s
Copyright (C) StarBrilliant <m13253@hotmail.com>
Released under GPL license version 3
#

mov	r15	pc
lda	r0	%9
add	r15	r0

lda	r0	%0x8000
ltt	r15	r0
#
Player 0: SU
Player 1: NS
#
ldasu	r14	%0
ldans	r14	%1
lda	r5	%9
lda	r9	%1
lda	r10	%0

# label 1 . #

lda	r0	%0x010b
lda	r1	%16
lsh	r0	r1
lda	r1	%0x100f
add	r0	r1
ldasu	r1	%0xe000
ldans	r1	%0x0000
sta	r0	@r1

lda	r0	%0xffff
lda	r1	%16
lsh	r0	r1
ldasu	r1	%0x0000
ldans	r1	%0xe000
sta	r0	@r1

# fork . . #
lda	r1	%0x1108
add	r1	r15
equ	r1	r10
addsu	r1	r9
lda	r0	%0xe000
equ	r1	r0
addsu	r1	r9
mov	r13	r1
mov	r11	r15
lda	r8	%48

# label 3 . #
lda	r0	@r11
sta	r0	@r1
add	r1	r9
add	r11	r9
sub	r8	r9
equ	r8	r10

# goto label 3 #
ldans	r0	%8
subns	pc	r0

equ	r0	r14
ldasu	r1	%0x8000
ldans	r1	%0x4000
add	r1	r3
lda	r8	%49

# label 2 . #
sta	r0	@r1
add	r1	r5
add	r3	r5
sub	r8	r9
equ	r8	r10

# goto label 2 #
ldans	r0	%7
subns	pc	r0
equ	r0	r14

# goto label 1 #
mov	r15	r13
mov	pc	r15

end	.	.
