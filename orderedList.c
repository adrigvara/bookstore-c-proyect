/*
 *  HEADER FILES
 */

#include <stdlib.h>
#include "orderedList.h"

/*
 *  SEARCH FUNCTION
 */

void* olSearch(OrderedList* list, int key){
  if(list==NULL || list->record.key<key)
    return NULL;
  if(list->record.key==key)
    return list->record.data;
  return olSearch(list->next, key);
}

/*
 *  INSERTION FUNCTION
 */

bool olInclude(OrderedList**, OrderedList*);
OrderedList* olAllocate(Record);

bool olInsert(OrderedList** listPointer, Record record){
  OrderedList* list = *listPointer;
  if(list==NULL || record.key<list->record.key)
    return olInclude(listPointer, olAllocate(record));
  if(record.key==list->record.key)
    return false;
  return olInsert(&list->next, record);
}
bool olInclude(OrderedList** listPointer, OrderedList* list){
  if(list==NULL)
    return false;
  list->next = *listPointer;
  *listPointer = list;
  return true;
}
OrderedList* olAllocate(Record record){
  OrderedList* list = (OrderedList*)calloc(1, sizeof(OrderedList));
  if(list!=NULL)
    list->record = record;
  return list;
}

/*
 *  DELETION FUNCTION
 */

OrderedList* olExclude(OrderedList**);
void* olDeallocate(OrderedList*);

void* olDelete(OrderedList** listPointer, int key){
  OrderedList* list = *listPointer;
  if(list==NULL || key<list->record.key)
    return NULL;
  if(list->record.key==key)
    return olDeallocate(olExclude(listPointer));
  return olDelete(&list->next, key);
}
OrderedList* olExclude(OrderedList** listPointer){
  OrderedList* list = *listPointer;
  *listPointer = list->next;
  return list;
}
void* olDeallocate(OrderedList* list){
  void* data = list->record.data;
  free(list);
  return data;
}

/*
 *  UTIL FUNCTIONS
 */

int olCount(OrderedList* list){
  if(list==NULL)
    return 0;
  return olCount(list->next)+1;
}
OrderedList* olSplit(OrderedList** listPointer, unsigned int index){
  if(*listPointer==NULL)
    return NULL;
  if(index==0){
    OrderedList* list = *listPointer;
    *listPointer = NULL;
    return list;
  }
  return olSplit(&(*listPointer)->next, index-1);
}
void olMerge(OrderedList** listPointer, OrderedList* list){
  if(*listPointer!=NULL)
    olMerge(&(*listPointer)->next, list);
  else
    *listPointer = list;
}
void olDeleteAll(OrderedList* list){
  if(list!=NULL){
    olDeleteAll(list->next);
    free(olDeallocate(list));
  }
}
Record createRecord(int key, void* data){
  Record record;
  record.key = key;
  record.data = data;
  return record;
}
