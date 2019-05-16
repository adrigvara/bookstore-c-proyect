#include <stdio.h>
#include <time.h>
#include "orderedList.c"
#include "bpTree.c"

int getOption(){
  int option;
  printf("%s\n", "[0] Salir");
  printf("%s\n", "[1] Imprimir arbol");
  printf("%s\n", "[2] Insertar clave");
  printf("%s\n", "[3] Eliminar clave");
  printf("%s\n", "[4] Eliminar arbol");
  printf("%s\n", "[5] Insertar claves (caso 1)");
  printf("%s\n", "[6] Insertar claves (caso 2)");
  printf("%s\n", "Ingrese la opcion deseada:");
  scanf("%i", &option);
  return option;
}
int getKey(){
  int key;
  printf("%s\n", "Ingrese la clave:");
  scanf("%i", &key);
  return key;
}
void printSpace(unsigned int length){
  if(length>0){
    printf("  ");
    printSpace(length-1);
  }
}
void pausePrinting(){
  printf("Presione <Enter> para continuar: ... \n");
  getchar();
  while(getchar()!='\n')
    ;
}
void printKey(int key, unsigned int space){
  printSpace(space);
  printf("%i\n", key);
}
void printLeaf(OrderedList* list, unsigned int space){
  if(list!=NULL){
    printLeaf(list->next, space);
    printKey(list->record.key, space);
  }
}
void printTree(BpTree tree, unsigned int space);
void printPage(BpTreePage* page, unsigned int space){
  if(page->next!=NULL){
    printPage(page->next->next, space);
    printKey(page->next->key, space);
  }
  printTree(page->tree, space+1);
}
void printTree(BpTree tree, unsigned int space){
  if(tree.leaf)
    printLeaf(tree.list, space);
  else
    printPage(tree.page, space);
}
void printKeys(BpTree root){
  system("clear");
  clock_t begin = clock();
  printTree(root, 0);
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("tiempo transcurrido %f\n",time_spent);
  pausePrinting();
}
void doOption(int option, BpTree* root){
  switch(option){
    case 1:
      printKeys(*root);
      break;
    case 2:
      bptInsertTree(root, createRecord(getKey(), NULL));
      break;
    case 3:
      bptDelete(root, getKey());
      break;
    case 4:
      bptDeleteAll(root);
      break;
    case 5:
      bptInsertTree(root, createRecord(10, NULL));
      bptInsertTree(root, createRecord(27, NULL));
      bptInsertTree(root, createRecord(29, NULL));
      bptInsertTree(root, createRecord(17, NULL));
      bptInsertTree(root, createRecord(25, NULL));
      bptInsertTree(root, createRecord(21, NULL));
      bptInsertTree(root, createRecord(15, NULL));
      bptInsertTree(root, createRecord(31, NULL));
      bptInsertTree(root, createRecord(13, NULL));
      bptInsertTree(root, createRecord(51, NULL));
      bptInsertTree(root, createRecord(20, NULL));
      bptInsertTree(root, createRecord(24, NULL));
      bptInsertTree(root, createRecord(48, NULL));
      bptInsertTree(root, createRecord(19, NULL));
      bptInsertTree(root, createRecord(60, NULL));
      bptInsertTree(root, createRecord(35, NULL));
      bptInsertTree(root, createRecord(66, NULL));
      break;
    case 6:
      bptInsertTree(root, createRecord(68, NULL));
      bptInsertTree(root, createRecord(80, NULL));
      bptInsertTree(root, createRecord(53, NULL));
      bptInsertTree(root, createRecord(10, NULL));
      bptInsertTree(root, createRecord(27, NULL));
      bptInsertTree(root, createRecord(43, NULL));
      bptInsertTree(root, createRecord(21, NULL));
      bptInsertTree(root, createRecord(77, NULL));
      bptInsertTree(root, createRecord(58, NULL));
      bptInsertTree(root, createRecord(63, NULL));
      bptInsertTree(root, createRecord(15, NULL));
      bptInsertTree(root, createRecord(37, NULL));
      bptInsertTree(root, createRecord(41, NULL));
      bptInsertTree(root, createRecord(72, NULL));
      bptInsertTree(root, createRecord(39, NULL));
      bptInsertTree(root, createRecord(95, NULL));
      bptInsertTree(root, createRecord(70, NULL));
      break;
  }
}

int main(int argc, char **argv){
  int option;
  BpTree root = bptCreateRoot();
  printf("%s\n", "Bienvenido al prototipo del proyecto!");
  while((option=getOption())!=0)
    doOption(option, &root);
  bptDeleteAll(&root);
  return 1;
}
