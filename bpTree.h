#ifndef __BP_TREE_H__
#define __BP_TREE_H__

/*
 *  HEADER FILES
 */

#include <stdbool.h>
#include "orderedList.h"

/*
 *  STRUCTS
 */

typedef struct BpTree BpTree;
typedef struct BpTreePage BpTreePage;
typedef struct BpTreeNode BpTreeNode;

struct BpTree{
  bool leaf;
  union{
    BpTreePage* page;
    OrderedList* list;
  };
};
struct BpTreePage{
  BpTree tree;
  BpTreeNode* prev;
  BpTreeNode* next;
};
struct BpTreeNode{
  int key;
  BpTreePage* prev;
  BpTreePage* next;
};

/*
 *  FUNCTIONS
 */

void* bptSearch(BpTree, int);
bool bptInsert(BpTree*, Record);
void* bptDelete(BpTree*, int);
BpTree bptCreateRoot();
void bptDeleteAll(BpTree*);

#endif
