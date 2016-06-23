#pragma once
#include <stdint.h>
#include "data_types.h"

/****** read single value start ******/

uint8_t read_uint8(const void *addr);

uint16_t read_uint16(const void *addr);

uint32_t read_uint32(const void *addr);

uint64_t read_uint64(const void *addr);

void read_buffer(const void *addr, void *dst, size_t len);

uint8_t read_variable_len(const void *addr, uint64_t *val);

/****** read single value end ******/


/****** read data type start ******/
void read_block_header(const void *addr, block_header_t *dst);

/****** read data type end ******/

