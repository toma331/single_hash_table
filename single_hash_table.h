/*
 * Single header hash map in C
 *
 * Usage:
 *  #define HASH_TABLE_IMPLEMENTATION
 *  #include "single_hash_table.h"
 *
 *  Author: Artem Koval
 *  Licence: MIT
 */
#ifndef SINGLE_HASH_TABLE_H
#define SINGLE_HASH_TABLE_H

#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char *key;
    char *value;
    struct Node *next;

} Node;

typedef struct hash_map {
    size_t num_of_elements;
    size_t capacity;

    Node **arr;

} hash_map;

void init_hash_map(hash_map *mp);
void init_hash_map_with_capacity(hash_map *mp, size_t capacity);
size_t hash_function(const hash_map *mp, const char *key);
void insert(hash_map *mp, const char *key, const char *value);
void hash_map_delete(hash_map *mp, const char *key);
void free_hash_map(hash_map *mp);
char *search(hash_map *mp, const char *key);

#endif

#ifdef HASH_TABLE_IMPLEMENTATION

#ifndef HASH_TABLE_INIT_CAPACITY
#define HASH_TABLE_INIT_CAPACITY 100
#endif

#ifndef HASH_TABLE_MAX_LOAD_PCT
#define HASH_TABLE_MAX_LOAD_PCT 75
#endif

static void set_node(Node *node, const char *key, const char *value)
{
    node->key = strdup(key);
    node->value = strdup(value);

    if (!node->key || !node->value) {
        free(node->key);
        free(node->value);
        node->key = node->value = NULL;
    }

    node->next = NULL;

    return;
}

static size_t hash_key(const char *key, size_t capacity)
{
    unsigned long hash = 0;

    for (const unsigned char *p = (const unsigned char *)key; *p; p++) {
        hash = hash * 31 + *p;
    }
    return hash % capacity;
}

static int hash_map_resize(hash_map *mp, size_t new_capacity)
{
    Node **new_arr = calloc(new_capacity, sizeof *new_arr);
    if (!new_arr)
        return 0;

    for (size_t i = 0; i < mp->capacity; i++) {
        Node *cur = mp->arr[i];
        while (cur) {
            Node *next = cur->next;
            size_t new_index = hash_key(cur->key, new_capacity);
            cur->next = new_arr[new_index];
            new_arr[new_index] = cur;
            cur = next;
        }
    }

    free(mp->arr);
    mp->arr = new_arr;
    mp->capacity = new_capacity;

    return 1;
}

void init_hash_map_with_capacity(hash_map *mp, size_t capacity)
{
    mp->capacity = capacity ? capacity : HASH_TABLE_INIT_CAPACITY;
    mp->num_of_elements = 0;
    mp->arr = calloc(mp->capacity, sizeof *mp->arr);
}

void init_hash_map(hash_map *mp)
{
    init_hash_map_with_capacity(mp, HASH_TABLE_INIT_CAPACITY);
}

size_t hash_function(const hash_map *mp, const char *key)
{
    return hash_key(key, mp->capacity);
}

void insert(hash_map *mp, const char *key, const char *value)
{
    if (mp->capacity == 0 || mp->arr == NULL) {
        if (!hash_map_resize(mp, HASH_TABLE_INIT_CAPACITY ? HASH_TABLE_INIT_CAPACITY : 1))
            return;
    }

    if ((mp->num_of_elements + 1) * 100 > mp->capacity * HASH_TABLE_MAX_LOAD_PCT) {
        size_t new_capacity = mp->capacity * 2;
        if (new_capacity > mp->capacity) {
            if (!hash_map_resize(mp, new_capacity))
                return;
        }
    }

    size_t bucketIndex = hash_function(mp, key);

    Node *newNode = malloc(sizeof *newNode);

    if (!newNode)
        return;

    set_node(newNode, key, value);

    if (!newNode->key) {
        free(newNode);
        return;
    }

    if (mp->arr[bucketIndex] == NULL) {
        mp->arr[bucketIndex] = newNode;
    }

    else {
        newNode->next = mp->arr[bucketIndex];
        mp->arr[bucketIndex] = newNode;
    }
    mp->num_of_elements++;
}

void hash_map_delete(hash_map *mp, const char *key)
{
    size_t bucketIndex = hash_function(mp, key);

    Node *prevNode = NULL;
    Node *currNode = mp->arr[bucketIndex];

    while (currNode != NULL) {
        if (strcmp(key, currNode->key) == 0) {
            if (currNode == mp->arr[bucketIndex]) {
                mp->arr[bucketIndex] = currNode->next;
            }

            else {
                prevNode->next = currNode->next;
            }

            free(currNode->key);
            free(currNode->value);
            free(currNode);

            mp->num_of_elements--;

            return;
        }

        prevNode = currNode;
        currNode = currNode->next;
    }
}

char *search(hash_map *mp, const char *key)
{
    size_t bucketIndex = hash_function(mp, key);
    Node *bucketHead = mp->arr[bucketIndex];

    while (bucketHead != NULL) {
        if (strcmp(bucketHead->key, key) == 0) {
            return bucketHead->value;
        }
        bucketHead = bucketHead->next;
    }

    return NULL;
}

void free_hash_map(hash_map *mp)
{
    for (size_t i = 0; i < mp->capacity; i++) {
        Node *cur = mp->arr[i];
        while (cur) {
            Node *next = cur->next;
            free(cur->key);
            free(cur->value);
            free(cur);
            cur = next;
        }
    }
    free(mp->arr);
}

#endif
