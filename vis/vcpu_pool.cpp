#include "vcpu_pool.h"

#include <cstring>

void vcpu_pool::add_vcpu (uint16_t int_pos, uint32_t color) {
	struct vcpu * vc = new struct vcpu;
	std::memset (vc, 0, sizeof(struct vcpu));
	vc->int_pos = int_pos;
	vc->color = color;
	pool.push_back(vc);
}
