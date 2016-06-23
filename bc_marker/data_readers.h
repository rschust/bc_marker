#pragma once
#include <stdint.h>
#include "data_types.h"

/****** read single value start ******/

#ifdef  __cplusplus
extern "C"
{
#endif //  __cplusplus

typedef uint8_t(* variable_len_reader_t) (void **src, void *dst);

void init_readers();

uint8_t read_uint8(void **src, void *dst);
uint8_t read_uint16(void **src, void *dst);
uint8_t read_uint32(void **src, void *dst);
uint8_t read_uint64(void **src, void *dst);

void read_buffer(void **src, void *dst, size_t len);

uint8_t read_variable_len(void **addr, void *dst);

uint8_t predict_variable_len(const unsigned char val);

variable_len_reader_t get_variable_len_reader(uint8_t type);

/****** read single value end ******/


/****** read data type start ******/

size_t read_block_header(const void * addr, size_t input_len, block_header_t * dst);

/****** read data type end ******/

#ifdef  __cplusplus
}
#endif //  __cplusplus
