/*
 *  HEADER FILES
 */

#include <stdlib.h>
#include "bpTree.h"

/*
 *  CONSTANTS
 */

#define BPT_ORDER 2
#define BPT_GREW_UP 'G'

/*
 *  SEARCH FUNCTION
 */

void* bptSearchPage(BpTreePage*, int);

void* bptSearch(BpTree tree, int key){
  if(tree.leaf)
    return olSearch(tree.list, key);
  return bptSearchPage(tree.page, key);
}
void* bptSearchPage(BpTreePage* page, int key){
  if(page->next==NULL || key<page->next->key)
    return bptSearch(page->tree, key);
  return bptSearchPage(page->next->next, key);
}

/*
 *  INSERT FUNCTION
 */

char bptInsertTree(BpTree*, Record);
char bptInsertPage(BpTreePage**, Record);
bool bptPageNeedSplit(BpTreePage*);
bool bptSplitPages(BpTree*);
bool bptLeafNeedSplit(OrderedList*);
bool bptSplitLeaves(BpTree*);
void bptReemplacePage(BpTreePage**);
BpTreePage* bptScrollPages(BpTreePage*, int);
int bptCountPages(BpTreePage*);
BpTreePage* bptAllocatePage(BpTree);
BpTree bptCreatePageTree(BpTreePage*);
BpTreeNode* bptExcludeNode(BpTreeNode*, int);
void bptLinkTree(BpTree*, BpTreePage*, BpTreeNode*, BpTreePage*);
BpTree bptCreateLeafTree(OrderedList*);
BpTreeNode* bptAllocateNode(int);
BpTreeNode* bptScrollNodes(BpTreeNode*, int);
void bptLinkPages(BpTreePage*, BpTreeNode*, BpTreePage*);

bool bptInsert(BpTree* root, Record record){
  return root!=NULL && record.data!=NULL && bptInsertTree(root, record);
}
char bptInsertTree(BpTree* tree, Record record){
  if(!tree->leaf){
    char flag = bptInsertPage(&tree->page, record);
    if(flag==BPT_GREW_UP){
      if(bptPageNeedSplit(tree->page))
        return bptSplitPages(tree) ? BPT_GREW_UP : false;
      else
        return true;
    }else
      return flag;
  }else{
    if(olInsert(&tree->list, record)){
      if(bptLeafNeedSplit(tree->list))
        return bptSplitLeaves(tree) ? BPT_GREW_UP : false;
      else
        return true;
    }
  }
  return false;
}
char bptInsertPage(BpTreePage** pagePointer, Record record){
  BpTreePage* page = *pagePointer;
  if(page->next==NULL || record.key<page->next->key){
    char flag = bptInsertTree(&page->tree, record);
    if(flag==BPT_GREW_UP)
      bptReemplacePage(pagePointer);
    return flag;
  }
  return bptInsertPage(&page->next->next, record);
}
bool bptPageNeedSplit(BpTreePage* page){
  return bptCountPages(page)>BPT_ORDER*2+1;
}
bool bptSplitPages(BpTree* tree){
  BpTreePage* page1 = tree->page;
  BpTreePage* page2 = bptScrollPages(tree->page, BPT_ORDER+1);
  page1 = bptAllocatePage(bptCreatePageTree(page1));
  page2 = bptAllocatePage(bptCreatePageTree(page2));
  if(page1==NULL || page2==NULL){
    free(page1);
    free(page2);
    return false;
  }
  BpTreeNode* node = bptExcludeNode(tree->page->next, BPT_ORDER);
  bptLinkTree(tree, page1, node, page2);
  return true;
}
bool bptLeafNeedSplit(OrderedList* list){
  return olCount(list)>BPT_ORDER*2;
}
bool bptSplitLeaves(BpTree* tree){
  OrderedList* list1 = tree->list;
  OrderedList* list2 = olSplit(&tree->list, BPT_ORDER);
  BpTreePage* page1 = bptAllocatePage(bptCreateLeafTree(list1));
  BpTreeNode* node = bptAllocateNode(list2->record.key);
  BpTreePage* page2 = bptAllocatePage(bptCreateLeafTree(list2));
  if(page1==NULL || node==NULL || page2==NULL){
    free(page1);
    free(node);
    free(page2);
    olMerge(&tree->list, list2);
    return false;
  }
  tree->leaf = false;
  bptLinkTree(tree, page1, node, page2);
  return true;
}
void bptReemplacePage(BpTreePage** pagePointer){
  BpTreePage* page = *pagePointer;
  *pagePointer = page->tree.page;
  if(page->prev!=NULL){
    page->tree.page->prev = page->prev;
    page->prev->next = page->tree.page;
  }
  if(page->next!=NULL){
    bptScrollPages(*pagePointer, 1)->next = page->next;
    page->next->prev = bptScrollPages(*pagePointer, 1);
  }
  free(page);
}
BpTreePage* bptScrollPages(BpTreePage* pages, int index){
  if(pages==NULL || index==0 || (index<0 && pages->prev==NULL) || (index>0 && pages->next==NULL))
    return pages;
  if(index<0)
    return bptScrollPages(pages->prev->prev, index+1);
  return bptScrollPages(pages->next->next, index-1);
}
int bptCountPages(BpTreePage* page){
  if(page==NULL)
    return 0;
  if(page->next==NULL)
    return 1;
  return bptCountPages(page->next->next)+1;
}
BpTreePage* bptAllocatePage(BpTree tree){
  BpTreePage* page = (BpTreePage*)calloc(1, sizeof(BpTreePage));
  if(page!=NULL)
    page->tree = tree;
  return page;
}
BpTree bptCreatePageTree(BpTreePage* page){
  BpTree tree;
  tree.leaf = false;
  tree.page = page;
  return tree;
}
BpTreeNode* bptExcludeNode(BpTreeNode* nodes, int index){
  BpTreeNode* node = bptScrollNodes(nodes, index);
  if(node!=NULL){
    node->prev->next = NULL;
    node->prev = NULL;
    node->next->prev = NULL;
    node->next = NULL;
  }
  return node;
}
void bptLinkTree(BpTree* tree, BpTreePage* page1, BpTreeNode* node, BpTreePage* page2){
  tree->page = page1;
  bptLinkPages(page1, node, page2);
}
BpTree bptCreateLeafTree(OrderedList* list){
  BpTree tree;
  tree.leaf = true;
  tree.list = list;
  return tree;
}
BpTreeNode* bptAllocateNode(int key){
  BpTreeNode* node = (BpTreeNode*)calloc(1, sizeof(BpTreeNode));
  if(node!=NULL)
    node->key = key;
  return node;
}
BpTreeNode* bptScrollNodes(BpTreeNode* nodes, int index){
  if(nodes==NULL || index==0)
    return nodes;
  if(index<0)
    return bptScrollNodes(nodes->prev->prev, index+1);
  return bptScrollNodes(nodes->next->next, index-1);
}
void bptLinkPages(BpTreePage* page1, BpTreeNode* node, BpTreePage* page2){
  page1->next = node;
  node->prev = page1;
  node->next = page2;
  page2->prev = node;
}

/*
 *  DELETE FUNCTION
 */

void* bptDeleteTree(BpTree*, int);
void* bptDeletePage(BpTreePage*, int);
bool bptNeedRestructure(BpTree);
void bptRestructure(BpTreePage*);
void bptRedistributeNextPage(BpTreePage*);
void bptMergePage(BpTreePage*);
void bptRedistributePrevPage(BpTreePage*);
void bptRedistributeNextLeaf(BpTreePage*);
void bptMergeLeaf(BpTreePage*);
void bptRedistributePrevLeaf(BpTreePage*);
BpTreePage* bptExcludeNextPage(BpTreePage*);
void bptSwapKeys(BpTreeNode*, BpTreeNode*);

void* bptDelete(BpTree* root, int key){
  void* data = NULL;
  if(root!=NULL){
    data = bptDeleteTree(root, key);
    if(!root->leaf && bptCountPages(root->page)==1){
      BpTreePage* page = root->page;
      *root = page->tree;
      free(page);
    }
  }
  return data;
}
void* bptDeleteTree(BpTree* tree, int key){
  if(!tree->leaf)
      return bptDeletePage(tree->page, key);
  return olDelete(&tree->list, key);
}
void* bptDeletePage(BpTreePage* page, int key){
  if(page->next!=NULL && key>=page->next->key)
    return bptDeletePage(page->next->next, key);
  void* data = bptDeleteTree(&page->tree, key);
  if(bptNeedRestructure(page->tree))
    bptRestructure(page);
  return data;
}
bool bptNeedRestructure(BpTree tree){
  if(!tree.leaf)
    return bptCountPages(tree.page)<BPT_ORDER+1;
  return olCount(tree.list)<BPT_ORDER;
}
void bptRestructure(BpTreePage* page){
  if(!page->tree.leaf){
    if(page->next!=NULL){
      if(bptCountPages(bptScrollPages(page, 1)->tree.page)>BPT_ORDER+1)
        bptRedistributeNextPage(page);
      else
        bptMergePage(page);
    }else{
      if(bptCountPages(bptScrollPages(page, -1)->tree.page)>BPT_ORDER+1)
        bptRedistributePrevPage(page);
      else
        bptMergePage(page->prev->prev);
    }
  }else{
    if(page->next!=NULL){
      if(olCount(bptScrollPages(page, 1)->tree.list)>BPT_ORDER)
        bptRedistributeNextLeaf(page);
      else
        bptMergeLeaf(page);
    }else{
      if(olCount(bptScrollPages(page, -1)->tree.list)>BPT_ORDER)
        bptRedistributePrevLeaf(page);
      else
        bptMergeLeaf(page->prev->prev);
    }
  }
}
void bptRedistributeNextPage(BpTreePage* page){
  BpTreePage* pageNext = page->next->next;
  pageNext->tree.page = pageNext->tree.page->next->next;
  pageNext = pageNext->tree.page->prev->prev;
  pageNext->next->next->prev = NULL;
  bptSwapKeys(page->next, pageNext->next);
  bptLinkPages(bptScrollPages(page->tree.page, BPT_ORDER-1), pageNext->next, pageNext);
  pageNext->next = NULL;
}
void bptMergePage(BpTreePage* page){
  BpTreePage* page2 = bptExcludeNextPage(page);
  bptLinkPages(bptScrollPages(page->tree.page, BPT_ORDER), page2->prev, page2->tree.page);
  free(page2);
}
void bptRedistributePrevPage(BpTreePage* page){
  BpTreePage* pagePrev = page->prev->prev;
  pagePrev = bptScrollPages(pagePrev->tree.page, bptCountPages(pagePrev->tree.page)-2);
  pagePrev = bptExcludeNextPage(pagePrev);
  bptSwapKeys(page->prev, pagePrev->prev);
  bptLinkPages(pagePrev, pagePrev->prev, page->tree.page);
  page->tree.page = pagePrev;
  pagePrev->prev = NULL;
}
void bptRedistributeNextLeaf(BpTreePage* page){
  OrderedList** listPointer = &page->next->next->tree.list;
  OrderedList* list = *listPointer;
  *listPointer = olSplit(listPointer, 1);
  olMerge(&page->tree.list, list);
  page->next->key = (*listPointer)->record.key;
}
void bptMergeLeaf(BpTreePage* page){
  BpTreePage* page2 = bptExcludeNextPage(page);
  olMerge(&page->tree.list, page2->tree.list);
  free(page2->prev);
  free(page2);
}
void bptRedistributePrevLeaf(BpTreePage* page){
  OrderedList** listPointer = &page->prev->prev->tree.list;
  OrderedList* list = olSplit(listPointer, olCount(*listPointer)-1);
  olMerge(&list, page->tree.list);
  page->tree.list = list;
  page->prev->key = list->record.key;
}
BpTreePage* bptExcludeNextPage(BpTreePage* page){
  BpTreePage* page2 = page->next->next;
  page->next = page2->next;
  if(page->next!=NULL)
    page->next->prev = page;
  return page2;
}
void bptSwapKeys(BpTreeNode* node1, BpTreeNode* node2){
  int aux = node1->key;
  node1->key = node2->key;
  node2->key = aux;
}

/*
 *  UTIL FUNCTIONS
 */

void bptDeleteTrees(BpTree);
void bptDeletePages(BpTreePage*);
void bptDeleteNodes(BpTreeNode*);

BpTree bptCreateRoot(){
  return bptCreateLeafTree(NULL);
}
void bptDeleteAll(BpTree *root){
  if(root!=NULL){
    bptDeleteTrees(*root);
    *root = bptCreateRoot();
  }
}
void bptDeleteTrees(BpTree tree){
  if(tree.leaf)
    olDeleteAll(tree.list);
  else
    bptDeletePages(tree.page);
}
void bptDeletePages(BpTreePage* pages){
  if(pages!=NULL){
    bptDeleteTrees(pages->tree);
    bptDeleteNodes(pages->next);
    free(pages);
  }
}
void bptDeleteNodes(BpTreeNode* nodes){
  if(nodes!=NULL){
    bptDeletePages(nodes->next);
    free(nodes);
  }
}
