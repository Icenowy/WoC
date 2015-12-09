#include "vcpu_pool.h"

#include <cstring>

struct vcpu * vcpu_pool::add_vcpu (uint16_t pc, uint16_t sp, uint16_t int_pos, uint32_t color) {
	struct vcpu * vc = new struct vcpu;
	std::memset (vc, 0, sizeof(struct vcpu));
	vc->int_pos = int_pos;
	vc->color = color;
	vc->registers[VCPU_REG_PC] = pc;
	vc->registers[VCPU_REG_SP] = sp;
	pool.push_back(vc);
	return vc;
}

void vcpu_pool::nexti () {
	std::vector<std::vector<struct vcpu*>::iterator> is;
	for(std::vector<struct vcpu*>::iterator i = pool.begin(); i < pool.end(); i++) {
		if (!vcpu_execute (*i, mem, mem_color)) {
			is.push_back(i);
		}
	}
	for(std::vector<struct vcpu*>::iterator i : is) {
		pool.erase(i);
	}
}
