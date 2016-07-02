#include "data_types.h"
#include "data_readers.h"
#include <stdio.h>
#include <Windows.h>

#define READER_BLOCK_SIZE 4096
#define BLOCK_FILENAME "f:/Bitcoin/blk00000.dat"
//#define BLOCK_FILENAME "D:/BitcoinData/blocks/blk00001.dat"
//#define BLOCK_FILENAME "C:/apps/code/bc_marker/block_chain_header.bin"

bool read_file(const char *path)
{
	//unsigned char buf[READER_BLOCK_SIZE];
	uint8_t *buf, *buf_current;
	FILE *fp = fopen(path, "rb");
	size_t bytes_read;
	size_t file_size;
	if (!fp) return false;

	fseek(fp, 0L, SEEK_END);
	file_size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	buf_current = buf = (uint8_t *)malloc(file_size);
	bc_reader_descriptor rd = { '\0' };
	while (bytes_read = fread(buf_current, 1, file_size, fp))
	{
		rd.pos = buf;
		block_t bl;
		read_block(&rd, file_size, &bl);
		buf_current += bytes_read;
	}
	//while (bytes_read = fread(buf, 1, READER_BLOCK_SIZE, fp))
	//{
	//	block_header_t header;
	//	read_block_header(buf, bytes_read, &header);

	//	if (bytes_read < READER_BLOCK_SIZE) break; // either the last buffer was read, or 0 was returned - either way, there is nothing to read
	//}

	fclose(fp);

	return true;
}

int main(int argc, char **argv)
{
	init_readers();
	read_file(BLOCK_FILENAME);

	return 0;
}
