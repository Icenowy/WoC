#ifndef _WOC_VM_VCPU_H
#define _WOC_VM_VCPU_H

#include <stdint.h>

#define VCPU_REG_PC 0x10
#define VCPU_REG_SP 0x11
#define VCPU_REG_INT 0x12

#define VCPU_INT_INV_INS 0x0

#if !defined(__cplusplus) && !defined(bool)
#define bool _Bool
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct vcpu {
	uint32_t registers[0x100];
	bool IN,SU; // flags
	uint32_t color_acc, color_exe;
	uint16_t int_pos, int_ret_pc;
};

// Return 0 if the CPU should halt, 1 if continue
bool vcpu_execute (struct vcpu *cpu, uint32_t *mem, uint32_t *mem_color);

#ifdef __cplusplus
}
#endif

#endif
