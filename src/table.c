#include "table.h"
#include "common.h"
#include "value.h"
#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#define TABLE_MAX_LOAD 0.7
void initTable(Table *table) {
  table->capacity = 0;
  table->count = 0;
  table->entries = NULL;
}

void freeTable(Table *table) {
  FREE_ARRAY(Entry, table->entries, table->capacity);
  initTable(table);
}
static Entry *findEntry(Entry *entries, int capacity, const ObjString *key) {
  uint32_t index = key->hash % capacity;
  Entry *tombStone = NULL;
  for (;;) {
    Entry *entry = &entries[index];
    bool foundEmpty = entry->key == NULL && IS_NIL(entry->value);
    bool foundTombStone = entry->key == NULL && !IS_NIL(entry->value);
    bool foundValue = entry->key != NULL;
    if (foundValue && entry->key == key)//Why compare the pointer,Shoudn't compare the values?
					//Lox has string interning,The strings only have different location,
					//if they are different,Otherwise they all contains same locations.
      return entry; 
                    
                    
    else if (foundEmpty)
      return tombStone != NULL ? tombStone : entry;
    else if (foundTombStone)
      tombStone = entry;
    index = (index + 1) % capacity;
  }
}

static void adjustCapacity(Table *table, int capacity) {
  Entry *entries = ALLOCATE(Entry, capacity);
  table->count = 0;
  for (int i = 0; i < capacity; i++) {
    entries[i].key = NULL;
    entries[i].value = NIL_VAL;
  }

  for (int i = 0; i < table->capacity; i++) {
    Entry *entry = &table->entries[i];
    if (entry->key == NULL)
      continue;

    Entry *dest = findEntry(entries, capacity, entry->key);
    dest->key = entry->key;
    dest->value = entry->value;
    table->count += 1;
  }
  FREE_ARRAY(Entry, table->entries, table->capacity);
  table->entries = entries;
  table->capacity = capacity;
}
bool tableSet(Table *table, ObjString *key, Value value) {
  if (table->count + 1 > table->capacity * TABLE_MAX_LOAD) {
    int capacity = GROW_CAPACITY(table->capacity);
    adjustCapacity(table, capacity);
  }

  Entry *entry = findEntry(table->entries, table->capacity, key);

  bool isNewKey = entry->key == NULL;
  if (isNewKey && IS_NIL(entry->value))
    table->count++;

  entry->key = key;
  entry->value = value;
  return isNewKey;
}

bool tableGet(const Table *table, const ObjString *key, Value *value) {
  if (table->count == 0)
    return false;
  Entry *searchedEntry = findEntry(table->entries, table->capacity, key);
  if (searchedEntry->key == NULL)
    return false;
  *value = searchedEntry->value;
  return true;
}

void tableAddAll(const Table *from, Table *to) {
  for (int i = 0; i < from->capacity; i++) {
    Entry *entry = &from->entries[i];
    if (entry->key != NULL)
      tableSet(to, entry->key, entry->value);
  }
}

bool tableDelete(Table *table, const ObjString *key) {
  if (table->count == 0)
    return false;
  Entry *entry = findEntry(table->entries, table->capacity, key);
  if (entry->key == NULL)
    return false;
  entry->key = NULL;
  entry->value = BOOL_VAL(true);
  return true;
}

ObjString *tableFindString(Table *table, const char *chars, uintptr_t length,
                           uint32_t hash) {
  if (table->count == 0)
    return NULL;
  uint32_t index = hash % table->capacity;
  for (;;) {
    Entry *entry = &table->entries[index];
    bool isEmpty = entry->key == NULL && IS_NIL(entry->value);
    // bool isTombStone = entry->key==NULL && !IS_NIL(entry->value);
    bool hasElement = entry->key != NULL;
    if (isEmpty)
      return NULL;
    else if (hasElement) {
      ObjString *key = entry->key;
      // Both string contains equal values
      if (key->length == length && key->hash == hash &&
          memcmp(key->chars, chars, length) == 0) {
        return entry->key;
      }
    }
    index = (index + 1) % table->capacity;
  }
}
