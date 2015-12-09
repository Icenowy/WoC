#ifndef _WOC_VIS_VCPU_POOL_H
#define _WOC_VIS_VCPU_POOL_H

#include "../vm/vcpu.h"

#include <vector>
#include <cstring>

struct vcpu_pool
{
	std::vector<struct vcpu *> pool;
	uint32_t mem[0x10000];
	uint32_t mem_color[0x10000];

	vcpu_pool () {
		std::memset (mem,0,sizeof (mem));
		std::memset (mem_color,0,sizeof (mem_color));
	}

	struct vcpu * add_vcpu (uint16_t pc, uint16_t sp, uint16_t int_pos, uint32_t color);
	void nexti ();
};
#endif
