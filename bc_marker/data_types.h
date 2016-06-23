#pragma once

#include <stdint.h>

typedef struct _block_header_t
{
	uint32_t magic_id;
	uint32_t header_length;
	uint32_t version_number;
	uint8_t prev_block_hash[32];
	uint8_t merkle_root_hash[32];
	uint32_t timestamp;
	uint32_t target_difficulty;
	uint32_t nonce;
	uint8_t v_transaction_count[9];
} block_header_t;

