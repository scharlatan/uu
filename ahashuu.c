/*unsorted uniq with hashes in an array, hashes are appended*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define INITLEN 256

int linsearch(long key, long* data, size_t len);
long* resize(long* data, size_t old, size_t new);

uint32_t hash(const char *key, uint32_t len, uint32_t seed);

int main(int argc, char** argv)
{
	size_t hashlen=INITLEN, pos=0;
	long hashval;
	char input[BUFSIZ];
	long* hashes=(long*)malloc(hashlen*sizeof(long));

	while(fgets(input, BUFSIZ, stdin)!=NULL)
	{
		hashval=hash(input, strlen(input), 0xA17A1111);

		if(linsearch(hashval, hashes, pos))
		{
			if(pos==hashlen)
			{
				hashlen*=2;
				hashes=resize(hashes, pos, hashlen);
				if(hashes==NULL)
					break;
			}
			hashes[pos++]=hashval;
			printf(input);
		}
	}

	free(hashes);

	return 0;
}

long* resize(long* data, size_t old, size_t new)
{
	size_t c;
	long* na=(long*)realloc(data, new*sizeof(long));
	if(na==NULL)
	{
		na=(long*)malloc(new*sizeof(long));
		if(na==NULL)
		{
			fprintf(stderr, "error: no memory left, exiting.\n");
			return NULL;
		}
		for(c=0; c<=old; c++)
			na[c]=data[c];
	}
	return na;
}

int linsearch(long key, long* data, size_t len)
{
	while(len-->0)
		if(data[len]==key)
			return 0;
	return 1;
}

uint32_t hash(const char *key, uint32_t len, uint32_t seed) {
	static const uint32_t c1 = 0xcc9e2d51;
	static const uint32_t c2 = 0x1b873593;
	static const uint32_t r1 = 15;
	static const uint32_t r2 = 13;
	static const uint32_t m = 5;
	static const uint32_t n = 0xe6546b64;

	uint32_t hash = seed;

	const int nblocks = len / 4;
	const uint32_t *blocks = (const uint32_t *) key;
	int i;
	for (i = 0; i < nblocks; i++)
	{
		uint32_t k = blocks[i];
		k *= c1;
		k = (k << r1) | (k >> (32 - r1));
		k *= c2;

		hash ^= k;
		hash = ((hash << r2) | (hash >> (32 - r2))) * m + n;
	}

	const uint8_t *tail = (const uint8_t *) (key + nblocks * 4);
	uint32_t k1 = 0;

	switch (len & 3)
	{
	case 3:
		k1 ^= tail[2] << 16;
	case 2:
		k1 ^= tail[1] << 8;
	case 1:
		k1 ^= tail[0];

		k1 *= c1;
		k1 = (k1 << r1) | (k1 >> (32 - r1));
		k1 *= c2;
		hash ^= k1;
	}

	hash ^= len;
	hash ^= (hash >> 16);
	hash *= 0x85ebca6b;
	hash ^= (hash >> 13);
	hash *= 0xc2b2ae35;
	hash ^= (hash >> 16);

	return hash;
}
