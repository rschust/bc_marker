#include "data_readers.h"
#include <string.h>

uint8_t read_uint8(const void *addr)
{
	uint8_t ret = *(uint8_t *)addr;
	return ret;
}

uint16_t read_uint16(const void *addr)
{
	uint16_t ret = *(uint16_t *)addr;
	return ret;
}

uint32_t read_uint32(const void *addr)
{
	uint32_t ret = *(uint32_t *)addr;
	return ret;
}

uint64_t read_uint64(const void *addr)
{
	uint64_t ret = *(uint64_t *)addr;
	return ret;
}

void read_buffer(const void *addr, void *dst, size_t len)
{
	memcpy(dst, addr, len);
}

uint8_t read_variable_len(const void *addr, uint64_t *val)
{
	uint8_t src = *(uint8_t *)addr;
	switch (0xFF - src)
	{
	case 0:
		*val = read_uint64(addr);
		return 9;
	case 1:
		*val = read_uint32(addr);
		return 5;
	case 2:
		*val = read_uint16(addr);
		return 3;
	default:
		*val = src;
		return 1;
	}
}

void read_block_header(const void * addr, block_header_t * dst)
{
	dst->magic_id = read_uint32(addr);
	dst->header_length = read_uint32(addr);
	dst->version_number = read_uint32(addr);
	read_buffer(addr, &dst->prev_block_hash, 32);
	read_buffer(addr, &dst->merkle_root_hash, 32);
	dst->timestamp = read_uint32(addr);
	dst->target_difficulty = read_uint32(addr);
	dst->nonce = read_uint32(addr);
}


