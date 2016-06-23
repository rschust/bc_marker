#include <WinSock2.h>
#include "data_readers.h"
#include <string.h>



#pragma comment(lib, "ws2_32.lib")

variable_len_reader_t g_readers[9];

void init_readers()
{
	g_readers[0] = &read_uint8;
	g_readers[2] = &read_uint16;
	g_readers[4] = &read_uint32;
	g_readers[8] = &read_uint64;
}

uint8_t read_uint8(void **src, void *dst)
{
	uint8_t **src_ptr = (uint8_t **)src;
	*(uint8_t *)dst = **(uint8_t **)src;
	++*src_ptr;
	return 1;
}

uint8_t read_uint16(void **src, void *dst)
{
	uint16_t **src_ptr = (uint16_t **)src;
	*(uint16_t *)dst = ntohs(**(uint16_t **)src);
	++*src_ptr;
	return 2;
}

uint8_t read_uint32(void **src, void *dst)
{
	uint32_t **src_ptr = (uint32_t **)src;
	*(uint32_t *)dst = ntohl(**(uint32_t **)src);
	++*src_ptr;
	return 4;
}

uint8_t read_uint64(void **src, void *dst)
{
	uint64_t **src_ptr = (uint64_t **)src;
	*(uint64_t *)dst = ntohll(**(uint64_t **)src);
	++*src_ptr;
	return 8;
}

variable_len_reader_t get_variable_len_reader(uint8_t type)
{
	return g_readers[type];
}

void read_buffer(void **src, void *dst, size_t len)
{
	memcpy(dst, *src, len);
	*(uint8_t **)src += len;
}

uint8_t read_variable_len(void **addr, void *dst)
{
	uint8_t *src_addr = *(uint8_t **)addr;
	uint8_t **src_addr_ptr = &src_addr;
	uint8_t len = predict_variable_len(*src_addr);
	
	if (len) ++*src_addr_ptr;
	uint64_t tmp_dst = 0;
	get_variable_len_reader(len)((void**)src_addr_ptr, &tmp_dst);
	*(uint64_t *)dst = tmp_dst;

	return len + 1;
}

uint8_t predict_variable_len(const unsigned char val)
{
	switch (0xFF ^ val)
	{
	case 0:
		return 8;
	case 1:
		return 4;
	case 2:
		return 2;
	default:
		return 0;
	}
}

size_t read_block_header(const void *addr, size_t input_len, block_header_t * dst)
{
	void *src = (void*) addr;
	void **src_ptr = &src;
	size_t ret = 88 + predict_variable_len(*((const unsigned char *)addr + 88));
	if (input_len >= ret)
	{
		read_uint32(src_ptr, &dst->magic_id);
		read_uint32(src_ptr, &dst->header_length);
		read_uint32(src_ptr, &dst->version_number);
		read_buffer(src_ptr, &dst->prev_block_hash, 32);
		read_buffer(src_ptr, &dst->merkle_root_hash, 32);
		read_uint32(src_ptr, &dst->timestamp);
		read_uint32(src_ptr, &dst->target_difficulty);
		read_uint32(src_ptr, &dst->nonce);
		read_variable_len(src_ptr, &dst->v_transaction_count);
	}

	return ret;
}


