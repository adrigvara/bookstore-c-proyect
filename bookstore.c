/*
 *  HEADER FILES
 */

#include "bookstore.h"
#include <string.h>
#include <stdlib.h>

/*
 *  SEARCH FUNCTION
 */

Book* bsSearch(BpTree bookstore, int isbn){
  return (Book*)bptSearch(bookstore, isbn);
}

/*
 *  INSERT FUNCTION
 */

Book* bsAllocate(Book book);
Record bsCreateBookRecord(Book book);

bool bsInsert(BpTree* bookstore, Book book){
  return bookstore!=NULL && bptInsert(bookstore, bsCreateBookRecord(book));
}
Record bsCreateBookRecord(Book book){
  return createRecord(book.isbn, bsAllocate(book));
}
Book* bsAllocate(Book book){
  Book* newBook = (Book*)malloc(sizeof(Book));
  if(newBook!=NULL){
    newBook->isbn = book.isbn;
    strcpy(newBook->title, book.title);
    strcpy(newBook->author, book.author);
    strcpy(newBook->editorial, book.editorial);
  }
  return newBook;
}

/*
 *  DELETE FUNCTION
 */

void bsDelete(BpTree* bookstore, int isbn){
  free(bptDelete(bookstore, isbn));
}
