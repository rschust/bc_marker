#pragma once

#include <stdint.h>

#ifdef  __cplusplus
extern "C"
{
#endif //  __cplusplus

typedef uint64_t variable_len_t;

typedef struct _block_header_t block_header_t;
typedef struct _transaction_input_t transaction_input_t;
typedef struct _transaction_output_t transaction_output_t;
typedef struct _transaction_t transaction_t;
typedef struct _block_t block_t;

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
	variable_len_t v_transaction_count;
} block_header_t;

typedef struct _transaction_input_t
{
	uint8_t transaction_hash[32];
	uint32_t transaction_index;
	variable_len_t v_script_len;
	uint8_t *raw_script;
	uint32_t sequence_number;
} transaction_input_t;

typedef struct _transaction_output_t
{
	uint64_t value_in_satoshi;
	variable_len_t v_script_len;
	uint8_t *public_key;
} transaction_output_t;

typedef struct _transaction_t
{
	uint32_t version_number;
	variable_len_t v_input_count;
	transaction_input_t *inputs;
	variable_len_t v_output_count;
	transaction_output_t *outputs;
	uint32_t lock_time;
} transaction_t;

typedef struct _block_t
{
	block_header_t header;
	transaction_t *transactions;
} block_t;

#ifdef  __cplusplus
}
#endif //  __cplusplus
