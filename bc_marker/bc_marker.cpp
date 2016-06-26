#include "data_types.h"
#include "data_readers.h"
#include <stdio.h>
#include <Windows.h>

#define BLOCK_FILENAME "D:/BitcoinData/blocks/blk00001.dat"
//#define BLOCK_FILENAME "C:/apps/code/bc_marker/block_chain_header.bin"

bool read_file(const char *path)
{
	const size_t block_size = 4096;
	unsigned char buf[block_size];
	FILE *fp = fopen(path, "rb");
	if (!fp) return false;

	for (size_t bytes_read; bytes_read = fread(buf, 1, block_size, fp); )
	{
		block_header_t header;
		read_block_header(buf, bytes_read, &header);
		break;
		if (bytes_read < block_size) break; // either the last buffer was read, or 0 was returned - either way, there is nothing to read
	}

	fclose(fp);

	return true;
}

int main(int argc, char **argv)
{
	init_readers();
	read_file(BLOCK_FILENAME);

	return 0;
}
