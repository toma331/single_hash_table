# Single-header Hash Map in C

A simple and lightweight **single-header hash map implementation in C**
using **separate chaining** for collision handling.

Easy to integrate — just include one file.

---

## Usage

To use the hash map, define `HASH_TABLE_IMPLEMENTATION` **once** before
including the header:

```c
#define HASH_TABLE_IMPLEMENTATION
#include "single_hash_table.h"
