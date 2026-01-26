/* Minimal usage example */

#define HASH_TABLE_IMPLEMENTATION
#include "single_hash_table.h"

#include <stdio.h>

int main(void) {

  hash_map map;
  init_hash_map(&map); 
  
  insert(&map, "apple", "red");
  insert(&map, "banana", "yellow");

  printf("apple: %s\n", search(&map, "apple")); 
  printf("banana: %s\n", search(&map, "banana")); 

  hash_map_delete(&map, "banana");
  printf("banana after delete: %s\n", search(&map, "banana"));

  free_hash_map(&map);

  return 0;
}
