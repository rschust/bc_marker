#include "data_types.h"
#include <stdio.h>
#include <Windows.h>

bool read_file(const char *path)
{
	const size_t block_size = 4096;
	unsigned char buf[block_size];
	FILE *fp = fopen(path, "rb");
	if (!fp) return false;

	for (;;)
	{
		size_t bytes_read = fread(buf, 1, block_size, fp);

		if (bytes_read < block_size) break; // either the last buffer was read, or 0 was returned - either way, there is nothing to read
	}

	fclose(fp);

	return true;
}

int main(int argc, char **argv)
{

	return 0;
}
