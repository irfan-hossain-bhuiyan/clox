#ifndef clox_table_h
#define clox_table_h
#include "value.h"

typedef struct{
	ObjString* key;
	Value value;
}Entry;

typedef struct{
	int count;
	int capacity;
	Entry* entries;
} Table;

void initTable(Table* table);
void freeTable(Table* table);
bool tableSet(Table* table, ObjString* key,Value value);
bool tableGet(const Table* table,const ObjString* key,Value* value);
bool tableDelete(Table* table,const ObjString* key);
void tableAddAll(const Table* from, Table* to);

#endif
