#ifndef __BOOKSTORE_H__
#define __BOOKSTORE_H__

/*
 *  HEADER FILES
 */

#include "bpTree.h"
#include <stdbool.h>

/*
 *  CONSTANTS
 */

#define BOOK_TITLE_LENGTH 80
#define BOOK_AUTHOR_LENGTH 100
#define BOOK_EDITORIAL_LENGTH 50

/*
 *  STRUCTS
 */

typedef struct Book{
  int isbn;
  char title[BOOK_TITLE_LENGTH];
  char author[BOOK_AUTHOR_LENGTH];
  char editorial[BOOK_EDITORIAL_LENGTH];
}Book;

/*
 *  FUNCTIONS
 */

Book* bsSearch(BpTree, int);
bool bsInsert(BpTree*, Book);
void bsDelete(BpTree*, int);
bool bsSave(BpTree, const char*);
bool bsLoad(BpTree*, const char*);

#endif
