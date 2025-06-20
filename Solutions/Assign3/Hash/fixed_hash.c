#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

// Returns the index where the key should be stored
size_t HashIndex(const char* key) {
    size_t sum = 0;
    for (const char* c = key; *c; c++) {
        sum += *c;
    }
    return sum % MAP_MAX; // Ensure index is within bounds
}

// Allocates memory for the HashMap
HashMap* HashInit() {
    HashMap* map = malloc(sizeof(HashMap));
    if (!map) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    // Initialize all pointers in the data array to NULL
    for (size_t i = 0; i < MAP_MAX; i++) {
        map->data[i] = NULL;
    }
    return map;
}

// Inserts PairValue into the map, if the value exists, increase ValueCount
void HashAdd(HashMap *map, PairValue *value) {
    if (!map || !value) return; // NULL check
    size_t idx = HashIndex(value->KeyName);
    if (map->data[idx])
        value->Next = map->data[idx]->Next;
    map->data[idx] = value;	
}

// Returns PairValue from the map if a given key is found
PairValue* HashFind(HashMap *map, const char* key) {
    if (!map || !key) return NULL; // NULL check
    size_t idx = HashIndex(key);
    for (PairValue* val = map->data[idx]; val != NULL; val = val->Next) {
        if (strcmp(val->KeyName, key) == 0) // Fix: Compare with == 0
            return val;
    }
    return NULL; 
}

// Deletes the entry with the given key from the map
void HashDelete(HashMap *map, const char* key) {
    if (!map || !key) return; // NULL check
    size_t idx = HashIndex(key);
    PairValue* val = map->data[idx];
    PairValue* prev = NULL;
    while (val) {
        if (strcmp(val->KeyName, key) == 0) { // Fix: Compare with == 0
            if (prev)
                prev->Next = val->Next;
            else
                map->data[idx] = val->Next;
            // Do not free val here, as it may be stack-allocated
            return;
        }
        prev = val;
        val = val->Next;
    }
}

// Prints all content of the map
void HashDump(HashMap *map) {
    if (!map) return; // NULL check
    for (size_t i = 0; i < MAP_MAX; i++) {
        for (PairValue* val = map->data[i]; val != NULL; val = val->Next) {
            printf("%s\n", val->KeyName); // Safe format string
        }
    }
}

int main() {
    HashMap* map = HashInit();
    printf("%s\n", "HashInit() Successful");
    
    // Dynamically allocate PairValue objects
    PairValue* pv1 = malloc(sizeof(PairValue));
    PairValue* pv2 = malloc(sizeof(PairValue));
    if (!pv1 || !pv2) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strncpy(pv1->KeyName, "test_key", KEY_STRING_MAX);
    pv1->ValueCount = 1;
    pv1->Next = NULL;
    strncpy(pv2->KeyName, "other_key", KEY_STRING_MAX);
    pv2->ValueCount = 1;
    pv2->Next = NULL;
    
    printf("HashAdd(map, '%s')\n", pv1->KeyName);
    HashAdd(map, pv1);

    printf("HashAdd(map, '%s')\n", pv1->KeyName);
    HashAdd(map, pv1);

    printf("HashAdd(map, '%s')\n", pv2->KeyName);
    HashAdd(map, pv2);

    printf("HashFind(map, %s) = ", pv1->KeyName);
    PairValue* result = HashFind(map, pv1->KeyName);
    if (result) {
        printf("{'%s': %d}\n", result->KeyName, result->ValueCount);
    }
    else {
        printf("%s\n", "Not found");
    }
    
    printf("%s", "HashDump(map) = ");
    HashDump(map);

    printf("HashDelete(map, '%s')\n", pv1->KeyName);
    HashDelete(map, pv1->KeyName);

    printf("HashFind(map, %s) = ", pv1->KeyName);
    result = HashFind(map, pv1->KeyName); 
    if (result) {
        printf("{'%s': %d}\n", result->KeyName, result->ValueCount);
    }
    else {
        printf("%s\n", "Not found");
    }

    printf("%s", "HashDump(map) = ");
    HashDump(map);

    // Free dynamically allocated memory
    free(pv1);
    free(pv2);
    free(map);
}