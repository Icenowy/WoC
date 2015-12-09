#include "vcpu.h"

#ifndef NDEBUG
#include <stdio.h>
#endif

static inline void extract_reg (uint32_t instruction, uint8_t *reg1, uint8_t *reg2)
{
	*reg1 = (uint8_t) ( (instruction & 0x0000FF00) >> 8);
	*reg2 = (uint8_t) (instruction & 0x000000FF);
}

#define TWIREG_INS_START do { \
	uint8_t reg1, reg2; \
	extract_reg (instruction, &reg1, &reg2); \
	uint32_t *preg1 = cpu->registers + reg1, *preg2 = cpu->registers + reg2;

#define TWIREG_INS_END } while (0); goto normal_exit;

#define COLOR_MEM_EXE(addr) if (mem_color) mem_color[addr] = cpu->color_exe;
#define COLOR_MEM_ACC(addr) if (mem_color) mem_color[addr] = cpu->color_acc;

_Bool vcpu_execute (struct vcpu *cpu, uint32_t *mem, uint32_t *mem_color)
{
	uint8_t interrupt_reason = 0;
	uint32_t instruction = mem[cpu->registers[VCPU_REG_PC] & 0xFFFF];
	COLOR_MEM_EXE (cpu->registers[VCPU_REG_PC] & 0xFFFF)
#ifndef NDEBUG
	fprintf (stderr, "VCPU %p executed instruction %08x\n", cpu, instruction);
#endif
	switch (instruction >> 29) {
	case 0:
		break;
	case 1:
		if(cpu->IN) break;
		goto normal_exit;
	case 2:
		if(cpu->SU) break;
		goto normal_exit;
	case 3:
		if(!cpu->SU) break;
		goto normal_exit;
	default:
		goto invalid_ins;
	}
	switch (instruction & 0x1F000000) {
	case 0x00000000:
		goto normal_exit;
	case 0x02000000: {
		// LDA REG %NUM
		uint8_t reg = (instruction & 0x00FF0000) >> 16;
		uint32_t num = (instruction & 0x0000FFFF);
		cpu->registers[reg] = num;
		break;
	}
	case 0x01000000:
		switch (instruction & 0x00FF0000) {
		case 0x00000000:
			goto normal_exit;
		case 0x00020000:
			// LDA REG @REG
			TWIREG_INS_START
			*preg1 = mem[ (*preg2) & 0xFFFF];
			COLOR_MEM_ACC ( (*preg2) & 0xFFFF);
			TWIREG_INS_END
		case 0x00030000:
			// STA REG @REG
			TWIREG_INS_START
			mem[ (*preg2) & 0xFFFF] = *preg1;
			COLOR_MEM_ACC ( (*preg2) & 0xFFFF);
			TWIREG_INS_END
		case 0x00040000:
			// EQU REG REG
			TWIREG_INS_START
			cpu->SU = *preg1 == *preg2;
			TWIREG_INS_END
		case 0x00050000:
			// BGT REG REG
			TWIREG_INS_START
			cpu->SU = *preg1 > *preg2;
			TWIREG_INS_END
		case 0x00060000:
			// LTT REG REG
			TWIREG_INS_START
			cpu->SU = *preg1 < *preg2;
			TWIREG_INS_END
		case 0x00070000:
			// ADD REG REG
			TWIREG_INS_START
			*preg1 += *preg2;
			TWIREG_INS_END
		case 0x00080000:
			// SUB REG REG
			TWIREG_INS_START
			*preg1 -= *preg2;
			TWIREG_INS_END
		case 0x00010000:
			switch (instruction & 0x0000FF00) {
			case 0x0000000:
				goto normal_exit;
			case 0x00000200:
				interrupt_reason = (uint8_t) (instruction & 0xFF);
				goto interrupt;
			case 0x00000300: {
				uint8_t reg = (uint8_t) (instruction & 0xFF);
				interrupt_reason = (uint8_t) (cpu->registers[reg] & 0xFF);
				goto interrupt;
			}
			}
		default:
			goto invalid_ins;
		}
	default:
		goto invalid_ins;
	}
normal_exit:
	cpu->registers[VCPU_REG_PC] = ( cpu->registers[VCPU_REG_PC] + 1) & 0xFFFF;
	return 1;
invalid_ins:
	interrupt_reason = VCPU_INT_INV_INS;
interrupt:
	if (cpu->IN) return 0;
	cpu->int_ret_pc = (uint16_t) (cpu->registers[VCPU_REG_PC] & 0xFFFF);
	cpu->registers[VCPU_REG_PC] = cpu->int_pos;
	cpu->registers[VCPU_REG_INT] = interrupt_reason;
	cpu->IN = 1;
	return 1;
}
