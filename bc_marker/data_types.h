#pragma once

#include <stdint.h>

#ifdef  __cplusplus
extern "C"
{
#endif //  __cplusplus

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
	uint64_t v_transaction_count; // this is variable length
} block_header_t;

#ifdef  __cplusplus
}
#endif //  __cplusplus
