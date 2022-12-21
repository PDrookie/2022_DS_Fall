#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SETBIT(a, n) (a[n/8] |= 1<<(n%8))
#define GETBIT(a, n) (a[n/8] & (1<<(n%8)))
#define H 7

// Hash function prototypes
unsigned int hash1(const char*);
unsigned int hash2(const char*);
unsigned int hash3(const char*);

// Number of bits in the filter
const int filter_size = 204800;

// Number of hash functions
const int num_hashes = 3;

// Bloom filter data structure
unsigned char filter[204800 / 8];

int main() {
// Add some keys to the filter
const char* keys[] = {"hello", "world", "foo", "bar", "baz", "haha", "meow"};
    for (int i = 0; i < H; i++) {
    // Hash the key using each of the hash functions
    unsigned int h1 = hash1(keys[i]) % filter_size;
    unsigned int h2 = hash2(keys[i]) % filter_size;
    unsigned int h3 = hash3(keys[i]) % filter_size;

    // Set the corresponding bits in the filter
    SETBIT(filter, h1);
    SETBIT(filter, h2);
    SETBIT(filter, h3);
    }
    // Check if a large number of keys are in the filter
    const char* pool[] = {"hell", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};
    int num_keys = 10000;
    int false_positives = 0;
    for (int i = 0; i < num_keys; i++) {
        // Generate a random key from the pool
        int index = rand() % 27;
        const char* key = pool[index];
        // Hash the key using each of the hash functions
        unsigned int h1 = hash1(key) % filter_size;
        unsigned int h2 = hash2(key) % filter_size;
        unsigned int h3 = hash3(key) % filter_size;

        // Check if the corresponding bits in the filter are set
        if (GETBIT(filter, h1) && GETBIT(filter, h2) && GETBIT(filter, h3)) {
        // Key is probably in the filter
        false_positives ++;
        }
    }

    // Calculate and print the false positive rate
    double false_positive_rate = false_positives / (double) num_keys;
    printf("%d\n", false_positives);
    printf("False positive rate: %f\n", false_positive_rate);

    return 0;
}

// Hash function implementations
unsigned int hash1(const char* s) {
unsigned int hash = 5381;
int c;
while ((c = *s++)) {
    hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

unsigned int hash2(const char* s) {
unsigned int hash = 0;
int c;
while ((c = *s++)) {
    hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

unsigned int hash3(const char* s) {
unsigned int hash = 0;
int c;
while ((c = *s++)) {
    hash = hash ^ c;
    }
    return hash;
}
