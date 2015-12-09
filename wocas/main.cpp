#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstdint>
#include <algorithm>

#define CONDITION_0 0x0
#define CONDITION_IN 0x1
#define CONDITION_SU 0x2

using namespace std;

typedef uint8_t reg;

reg s2reg (string n)
{
#define R(name,val) if (n == #name) return val;
	R(R0,0x0)
	R(R1,0x1)
	R(R2,0x2)
	R(R3,0x3)
	R(R4,0x4)
	R(R5,0x5)
	R(R6,0x6)
	R(R7,0x7)
	R(R8,0x8)
	R(R9,0x9)
	R(R10,0xA)
	R(R11,0xB)
	R(R12,0xC)
	R(R13,0xE)
	R(R14,0xD)
	R(R15,0xF)
	R(PC,0x10)
	R(SP,0x11)
#undef R
	throw 0;
}

#define TRY try {
#define ETRY } catch (int) { goto invalid; }

uint32_t gen_instruction (string s1, string s2, string s3)
{
	uint8_t condition = CONDITION_0;
	if (s1.length () != 3 && s1.length () != 5) goto invalid;
	if (s1.length () == 5) {
		string scond = s1.substr (3,2);
		s1 = s1.substr (0,3);
		if (scond == "IN") condition = CONDITION_IN;
		else if (scond == "SU") condition = CONDITION_SU;
		else goto invalid;
	}
#ifndef NDEBUF
	cerr << "Assembling instruction: " << s1 << " " << s2 << " " << s3 << endl;
#endif
	if (s1 == "LDA") {
		if (s3[0] == '%') {
			// LDA REG %NUM
			uint32_t num = stoul (s3.substr (1));
			reg r;
			TRY
				r = s2reg (s2);
			ETRY
			return ( 0x02000000 | (condition << 29) | (r << 16) | (num & 0xFFFF) );
		}
		else if (s3[0] == '@') {
			// LDA REG @REG
			reg r1, r2;
			TRY
				r1 = s2reg (s2);
				r2 = s2reg (s3.substr (1));
			ETRY
			return ( 0x01020000 | (condition << 29) | (r1 << 8) | r2 );
		}
		else goto invalid;
	}
	else if (s1 == "STA") {
		if (s3[0] == '@') {
			reg r1, r2;
			TRY
				r1 = s2reg (s2);
				r2 = s2reg (s3.substr (1));
			ETRY
			return ( 0x01030000 | (condition << 29) | (r1 << 8) | r2 );
		}
		else goto invalid;
	}
#define TWIREG_INS(name, opcode) else if (s1 == #name) { \
	reg r1, r2; \
	TRY \
	r1 = s2reg (s2); \
	r2 = s2reg (s3); \
	ETRY \
	return ( opcode | (condition << 29) | (r1 << 8) | r2 ); \
}
	TWIREG_INS(ADD, 0x01070000)
	TWIREG_INS(SUB, 0x01080000)
	TWIREG_INS(EQU, 0x01040000)
	TWIREG_INS(BGT, 0x01050000)
	TWIREG_INS(LTT, 0x01060000)
	else if (s1 == "INT") {
		if (s2[0] == '%') {
			uint32_t num = stoul (s2.substr (1));
			return ( 0x01010200 | (condition << 29) | (num & 0xFF) );
		}
		else {
			reg r;
			TRY
			r = s2reg (s2);
			ETRY
			return ( 0x01010300 | (condition << 29) | r );
		}
	}
invalid:
	cerr << "Invalid instruction: " << s1 << " " << s2 << " " << s3 << endl;
	exit(1);
	return 0; // never run to here
}

char op_toupper (char c) { return toupper(c); }

string stoupper (string s)
{
	transform (s.begin (), s.end (), s.begin (), op_toupper);
	return s;
}

int main()
{
	while(true) {
		string s1, s2, s3;
		cin >> s1 >> s2 >> s3;
		s1 = stoupper (s1);
		s2 = stoupper (s2);
		s3 = stoupper (s3);
		if(s1 == "END") return 0;
		uint32_t u = gen_instruction (s1, s2, s3);
		fwrite (&u, sizeof(uint32_t), 1, stdout);
	}
}
