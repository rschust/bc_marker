#pragma once
#include <stdint.h>
#include "data_types.h"

/****** read single value start ******/

#ifdef  __cplusplus
extern "C"
{
#endif //  __cplusplus

typedef uint8_t(* variable_len_reader_t) (void **src, void *dst);

typedef struct _bc_reader_descriptor
{
	uint8_t *pos;
	uint8_t *buf;
} bc_reader_descriptor;

void init_readers();

uint8_t read_uint8(void *src, void *dst);
uint8_t read_uint16(void *src, void *dst);
uint8_t read_uint32(void *src, void *dst);
uint8_t read_uint64(void *src, void *dst);
//uint8_t read_variable_len(void *addr, void *dst);

void read_buffer(void *src, void *dst, size_t len);

uint8_t predict_variable_len(const unsigned char val);

variable_len_reader_t get_variable_len_reader(uint8_t type);

uint8_t bc_read_uint8(bc_reader_descriptor *rd, void *dst);
uint8_t bc_read_uint16(bc_reader_descriptor *rd, void *dst);
uint8_t bc_read_uint32(bc_reader_descriptor *rd, void *dst);
uint8_t bc_read_uint64(bc_reader_descriptor *rd, void *dst);
uint8_t bc_read_variable_len(bc_reader_descriptor *rd, void *dst);
void bc_read_buffer(bc_reader_descriptor *rd, void *dst, size_t len);


/****** read single value end ******/


/****** read data type start ******/

size_t read_block_header(bc_reader_descriptor *rd, size_t input_len, block_header_t * dst);
size_t read_input(bc_reader_descriptor *rd, transaction_input_t * dst);
size_t read_inputs(bc_reader_descriptor *rd, transaction_t * dst);
size_t read_transaction(bc_reader_descriptor *rd, transaction_t * dst);
size_t read_block(bc_reader_descriptor *rd, size_t input_len, block_t * dst);

/****** read data type end ******/

#ifdef  __cplusplus
}
#endif //  __cplusplus
