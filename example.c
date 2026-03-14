/* Basic tests and usage example */

#define HASH_TABLE_IMPLEMENTATION
#include "single_hash_table.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

static void expect_true(const char *label, int cond)
{
    if (!cond) {
        printf("FAIL: %s\n", label);
        failures++;
    } else {
        printf("ok: %s\n", label);
    }
}

static void expect_str_eq(const char *label, const char *got, const char *want)
{
    if (got == NULL || want == NULL) {
        expect_true(label, got == want);
        return;
    }
    expect_true(label, strcmp(got, want) == 0);
}

static void expect_null(const char *label, const void *ptr)
{
    expect_true(label, ptr == NULL);
}

static void expect_not_null(const char *label, const void *ptr)
{
    expect_true(label, ptr != NULL);
}

int main(void)
{
    hash_map map;
    init_hash_map(&map);

    expect_not_null("init allocates bucket array", map.arr);
    if (!map.arr) {
        printf("FATAL: allocation failed\n");
        return 1;
    }

    expect_true("init sets size to 0", map.num_of_elements == 0);
    expect_true("init sets capacity to 100", map.capacity == 100);

    insert(&map, "apple", "red");
    insert(&map, "banana", "yellow");

    expect_str_eq("search finds apple", search(&map, "apple"), "red");
    expect_str_eq("search finds banana", search(&map, "banana"), "yellow");

    insert(&map, "apple", "green");
    expect_str_eq("search returns latest value", search(&map, "apple"), "green");
    expect_true("size counts inserts (including duplicates)", map.num_of_elements == 3);

    hash_map_delete(&map, "apple");
    expect_str_eq("delete removes first match", search(&map, "apple"), "red");
    expect_true("size decremented after delete", map.num_of_elements == 2);

    hash_map_delete(&map, "banana");
    expect_null("deleted key not found", search(&map, "banana"));
    expect_true("size decremented after delete", map.num_of_elements == 1);

    hash_map_delete(&map, "missing");
    expect_true("delete missing key is no-op", map.num_of_elements == 1);

    free_hash_map(&map);

    printf("\n%s: %d failure(s)\n", failures ? "FAILED" : "PASSED", failures);
    return failures ? 1 : 0;
}
