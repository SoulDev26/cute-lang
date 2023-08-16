#ifndef CUTE_MURMUR3_32_HASH_H
#define CUTE_MURMUR3_32_HASH_H

#include <stddef.h>
#include <string.h>

#include "int_types.h"
#include "vm/logging.h"

#define CUTE_MURMUR3_32_SEED 0x12345678


u32 cute_murmur32_scramble(u32 k);
u32 cute_murmur3_32_hash(const char* str);


#endif // CUTE_MURMUR3_32_HASH_H
