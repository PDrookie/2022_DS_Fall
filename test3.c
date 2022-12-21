#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define DATA_N 1000000
#define FILTER_BYTE 2048000
#define H 20

static unsigned char filter[FILTER_BYTE];

/* Reference https://github.com/abrandoned/murmur2/blob/master/MurmurHash2.c */
unsigned int MurmurHash2(int key, unsigned int seed) {
  size_t len = 4;
  const unsigned int m = 0x5bd1e995;
  const int r = 24;
  unsigned int h = seed ^ len;
  /* Mix 4 bytes at a time into the hash */
  const unsigned char *data = &key;
  while (len >= 4) {
    unsigned int k = *(unsigned int *)data;
    k *= m;
    k ^= k >> r;
    k *= m;
    h *= m;
    h ^= k;
    data += 4;
    len -= 4;
  }
  /* Handle the last few bytes of the input array  */
  switch (len) {
    case 3:
      h ^= data[2] << 16;
    case 2:
      h ^= data[1] << 8;
    case 1:
      h ^= data[0];
      h *= m;
  };
  /* Do a few final mixes of the hash to ensure the last few
  // bytes are well-incorporated.  */
  h ^= h >> 13;
  h *= m;
  h ^= h >> 15;
  return h % (FILTER_BYTE * 8);
}

void insert_key(int key, int h) {
  for (int i = 0; i < h; i++) {
    unsigned int bit = MurmurHash2(key, i << 5);
    filter[(int)(bit / 8)] |= 1 << (bit % 8);
  }
}

unsigned char have_key(int key, int h) {
  for (int i = 0; i < h; i++) {
    unsigned int bit = MurmurHash2(key, i << 5);
    if ((filter[(int)(bit / 8)] & (1 << (bit % 8))) == 0) {
      return 0;
    }
  }
  return 1;
}

int main() {
  int seed = time(NULL);
  for (int i = 1; i < H + 1; i++) {
    srand(seed);
    int count = 0;
    memset(filter, 0, FILTER_BYTE);
    for (int j = 0; j < DATA_N; j++)
      insert_key(rand() * 1000 * 2 + 1, i);

    for (int j = 0; j < DATA_N; j++)
      count += have_key(rand() * 1000 * 2, i);
    printf("%d,%f,%f\n", i, (float)count / DATA_N, pow(1-pow(1-1.0/(FILTER_BYTE*8), DATA_N*i), i));
  }
  return 0;
}