#include <WinSock2.h>
#include "data_readers.h"
#include <string.h>

#define BC_LITTLE_ENDIAN

#ifdef BC_LITTLE_ENDIAN
#define read_short(x)		(x)
#define read_long(x)		(x)
#define read_longlong(x)	(x)
#else

#define read_short(x) ntohs(x)
#define read_long(x) ntohl(x)
#define read_longlong(x) ntohll(x)

#pragma comment(lib, "ws2_32.lib")

#endif

#define BC_ALLOC(x) malloc(x)
#define BC_FREE(x) free(x)

variable_len_reader_t g_readers[9];

void init_readers()
{
	g_readers[0] = &bc_read_uint8;
	g_readers[2] = &bc_read_uint16;
	g_readers[4] = &bc_read_uint32;
	g_readers[8] = &bc_read_uint64;
}

uint8_t read_uint8(void *src, void *dst)
{
	*(uint8_t *)dst = *(uint8_t *)src;
	return 1;
}

uint8_t read_uint16(void *src, void *dst)
{
	*(uint16_t *)dst = read_short(*(uint16_t *)src);
	return 2;
}

uint8_t read_uint32(void *src, void *dst)
{
	*(uint32_t *)dst = read_long(*(uint32_t *)src);
	return 4;
}

uint8_t read_uint64(void *src, void *dst)
{
	*(uint64_t *)dst = read_longlong(*(uint64_t *)src);
	return 8;
}

uint8_t bc_read_uint8(bc_reader_descriptor *rd, void *dst)
{
	uint8_t bytes_read = read_uint8(rd->pos, dst);
	rd->pos += bytes_read;
	return bytes_read;
}

uint8_t bc_read_uint16(bc_reader_descriptor *rd, void *dst)
{
	uint8_t bytes_read = read_uint16(rd->pos, dst);
	rd->pos += bytes_read;
	return bytes_read;
}

uint8_t bc_read_uint32(bc_reader_descriptor *rd, void *dst)
{
	uint8_t bytes_read = read_uint32(rd->pos, dst);
	rd->pos += bytes_read;
	return bytes_read;
}

uint8_t bc_read_uint64(bc_reader_descriptor *rd, void *dst)
{
	uint8_t bytes_read = read_uint64(rd->pos, dst);
	rd->pos += bytes_read;
	return bytes_read;
}

void bc_read_buffer(bc_reader_descriptor *rd, void *dst, size_t len)
{
	memcpy(dst, rd->pos, len);
	rd->pos += len;
}

uint8_t read_variable_len(bc_reader_descriptor *rd, void *dst)
{
}

variable_len_reader_t get_variable_len_reader(uint8_t type)
{
	return g_readers[type];
}

void read_buffer(void *src, void *dst, size_t len)
{
	memcpy(dst, src, len);
}

uint8_t bc_read_variable_len(bc_reader_descriptor *rd, void *dst)
{
	uint8_t len = predict_variable_len(*rd->pos);
	
	if (len) ++rd->pos;
	uint64_t tmp_dst = 0;
	get_variable_len_reader(len)(rd, &tmp_dst);
	*(uint64_t *)dst = tmp_dst;

	return len + 1;
}

uint8_t predict_variable_len(const unsigned char val)
{
	switch (~val)
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

size_t read_block_header(bc_reader_descriptor *rd, size_t input_len, block_header_t * dst)
{
	size_t ret = 89 + predict_variable_len(*((const unsigned char *)rd->pos + 88));
	if (input_len >= ret)
	{
		bc_read_uint32(rd, &dst->magic_id);
		bc_read_uint32(rd, &dst->header_length);
		bc_read_uint32(rd, &dst->version_number);
		bc_read_buffer(rd, dst->prev_block_hash, 32);
		bc_read_buffer(rd, dst->merkle_root_hash, 32);
		bc_read_uint32(rd, &dst->timestamp);
		bc_read_uint32(rd, &dst->target_difficulty);
		bc_read_uint32(rd, &dst->nonce);
		bc_read_variable_len(rd, &dst->v_transaction_count);
	}

	return ret;
}

size_t read_input(bc_reader_descriptor *rd, transaction_input_t * dst)
{
	bc_read_buffer(rd, dst->transaction_hash, 32);
	bc_read_uint32(rd, &dst->transaction_index);
	bc_read_variable_len(rd, &dst->v_script_len);
	dst->raw_script = BC_ALLOC(dst->v_script_len);
	bc_read_buffer(rd, dst->raw_script, 32);
	bc_read_uint32(rd, &dst->sequence_number);
}

size_t read_inputs(bc_reader_descriptor *rd, transaction_t * dst)
{
	size_t i;
	dst->inputs = BC_ALLOC(sizeof(transaction_input_t) * dst->v_input_count);
	for (i = 0; i < dst->v_input_count; ++i)
	{
		read_input(rd, dst->inputs + i);
	}
}

size_t read_transaction(bc_reader_descriptor *rd, transaction_t * dst)
{
	bc_read_uint32(rd, &dst->version_number);
	bc_read_variable_len(rd, &dst->v_input_count);
	read_inputs(rd, dst);
}

size_t read_block(bc_reader_descriptor *rd, size_t input_len, block_t * dst)
{
	size_t offset;
	size_t i;
	offset = read_block_header(rd, input_len, &dst->header);
	dst->transactions = BC_ALLOC(sizeof(transaction_input_t) * dst->header.v_transaction_count);
	for (i = 0; i < dst->header.v_transaction_count; ++i)
	{
		read_transaction(rd, dst->transactions + i);
	}
}
