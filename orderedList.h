#ifndef __ORDERED_LIST_H__
#define __ORDERED_LIST_H__

/*
 *  HEADER FILES
 */

#include <stdbool.h>

/*
 *  STRUCTS
 */

typedef struct Record Record;
typedef struct OrderedList OrderedList;

struct Record{
  int key;
  void* data;
};
struct OrderedList{
  Record record;
  OrderedList* next;
};

/*
 *  FUNCTIONS
 */

void* olSearch(OrderedList*, int);
bool olInsert(OrderedList**, Record);
void* olDelete(OrderedList**, int);
int olCount(OrderedList*);
OrderedList* olSplit(OrderedList**, unsigned int);
void olMerge(OrderedList**, OrderedList*);
void olDeleteAll(OrderedList*);
Record createRecord(int, void*);

#endif
