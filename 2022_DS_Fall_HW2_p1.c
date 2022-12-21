#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define SETBIT(a, n) (a[n/8] |= 1 << (n % 8))
#define GETBIT(a, n) (a[n/8] & (1 << (n % 8)))
#define H 50
#define M 1000000
#define U 100000
#define filter_Bytes 1048576

int c;

const int filter_size = 1 << 20;
const int num_hashes = 3;
unsigned char filter[filter_Bytes / 8];

// Hash function implementations
unsigned int hash1(const char* s, unsigned int hash) {
while ((c = *s++)) {
    hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

int main() {
   for(int j = 1; j <= H; j ++){
      int key;
      int count = 0;
      srand(time(NULL));
      for (int i = 0; i < U; i++) {
         key = rand() * 1000 * 2 % M;
         const unsigned char *data = &key;
         unsigned int h1 = hash1(data, j << 5) % filter_size;   
         SETBIT(filter, h1);
      }
      for (int i = 0; i < U; i++) {
         int index = rand() * 1000 * 2 + 1 % M;
         const unsigned char* key = &index;
         unsigned int h1 = hash1(key, j << 5) % filter_size;
         if (GETBIT(filter, h1)) {
            count ++;
         }
      }
      double false_positive_rate = pow(1 - pow(1 - (1.0 / M), U * j), j);
      //printf("(%d) False positive rate: %f ; ", j + 1, (float)count / U);
      printf(" %f\n", false_positive_rate);
   }
   return 0;
}