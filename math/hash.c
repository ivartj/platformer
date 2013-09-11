#include "hash.h"

uint64_t hash_create(unsigned char *data, size_t len)
{
	uint64_t hash;
	int hashlen;
	unsigned char *bytes;
	int i;

	hash = 0;
	hashlen = sizeof(hash);
	bytes = (unsigned char *)&hash;

	for(i = 0; i < len; i++)
		bytes[i % hashlen] ^= data[i];
	
	return hash;
}
