/*
 *  HEADER FILES
 */

#include <stdbool.h>
#include <time.h>
#include <GL/glui.h>
#include <GL/glut.h>
#include "orderedList.c"
#include "bpTree.c"
#include "bookstore.c"

/*
 *  CONSTANTS
 */

#define CELL_SIDE_LENGTH 1.0
enum{
  CLEAR_FORM = 0,
  SEARCH_BOOK,
  INSERT_BOOK,
  DELETE_BOOK,
  DELETE_ALL
};
enum{
  RED_COLOR = 0,
  YELLOW_COLOR,
  BLACK_COLOR,
  GRAY_COLOR
};

/*
 *  GLOBAL VARIABLES
 */

Book book;
int search;
int mainWindow;
GLUI_StaticText* timer;
BpTree bookstore = {true, NULL};
float translation[3] = {0.0, 0.0, -15.0};

/*
 *  CALLBACK FUNCTIONS
 */

double getTimeElapsed(clock_t begin){
  return (double)(clock() - begin)/CLOCKS_PER_SEC;
}
void updateTimer(clock_t begin){
  char timeElapsed[50];
  sprintf(timeElapsed, "%f", getTimeElapsed(begin));
  timer->set_text(timeElapsed);
}
void searchBook(){
  clock_t begin = clock();
  Book* bookFound = bsSearch(bookstore, book.isbn);
  updateTimer(begin);
  if(bookFound){
    book.isbn = bookFound->isbn;
    strcpy(book.title, bookFound->title);
    strcpy(book.author, bookFound->author);
    strcpy(book.editorial, bookFound->editorial);
  }else{
    book.title[0] = '\0';
    book.author[0] = '\0';
    book.editorial[0] = '\0';
  }
}
 void idle(){
 	glutSetWindow(mainWindow);
  if(search)
    searchBook();
  GLUI_Master.sync_live_all();
 	glutPostRedisplay();
}
void special(int key, int x, int y){
  switch(key){
    case GLUT_KEY_LEFT:
    translation[0]++;
    break;
    case GLUT_KEY_RIGHT:
    translation[0]--;
    break;
    case GLUT_KEY_DOWN:
    translation[1]++;
    break;
    case GLUT_KEY_UP:
    translation[1]--;
    break;
  }
}
void button(int id){
  clock_t begin = clock();
  switch(id){
    case CLEAR_FORM:
    book.isbn = 0;
    book.title[0] = '\0';
    book.author[0] = '\0';
    book.editorial[0] = '\0';
    timer->set_text("0");
    search = 0;
    break;
    case INSERT_BOOK:
    bsInsert(&bookstore, book);
    break;
    case DELETE_BOOK:
    bsDelete(&bookstore, book.isbn);
    break;
    case DELETE_ALL:
    bptDeleteAll(&bookstore);
    break;
  }
  updateTimer(begin);
}

/*
 *  DISPLAY FUNCTION
 */

float displayTree(bool search, BpTree, float x, float y);
float displayLeaf(bool search, OrderedList*, float x, float y);
float displayPage(bool search, BpTreePage*, float x, float y);
float displayNode(bool search, BpTreeNode* node, float x, float y);
void displayKey(int color, int key, float x, float y);
void displayQuad(float x, float y);
void displayEdge(float x, float y);
void displayNumber(int number, float x, float y);
void displayVertex(float x, float y);
void changeColor(int color);

void display(){
  glutSetWindow(mainWindow);
  glClearColor(0, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();
  glTranslatef(translation[0], translation[1], translation[2]);
  displayTree(search, bookstore, 0, 0);
  glutSwapBuffers();
}
float displayTree(bool search, BpTree tree, float x, float y){
  if(tree.leaf)
    return displayLeaf(search, tree.list, x, y);
  else
    return displayPage(search, tree.page, x, y);
}
float displayLeaf(bool search, OrderedList* list, float x, float y){
  if(list==NULL)
    return x;
  int color = !search || list->record.key>book.isbn ? GRAY_COLOR : list->record.key==book.isbn ? YELLOW_COLOR : RED_COLOR;
  displayKey(color, list->record.key, x, y);
  return displayLeaf(search, list->next, x+CELL_SIDE_LENGTH, y);
}
float displayPage(bool search, BpTreePage* page, float x, float y){
  bool flag = search && (page->next==NULL || book.isbn<page->next->key);
  return displayNode(search, page->next, displayTree(flag, page->tree, x, y-CELL_SIDE_LENGTH*2), y);
}
float displayNode(bool search, BpTreeNode* node, float x, float y){
  if(node==NULL)
    return x;
  displayKey(search ? RED_COLOR : GRAY_COLOR, node->key, x, y);
  return displayPage(search && book.isbn>=node->key, node->next, x+CELL_SIDE_LENGTH, y);
}
void displayKey(int color, int key, float x, float y){
  changeColor(color);
  displayQuad(x, y);
  changeColor(BLACK_COLOR);
  displayEdge(x, y);
  displayNumber(key, x, y);
}
void changeColor(int color){
  switch(color){
    case GRAY_COLOR:
    glColor3f(0.5, 0.5, 0.5);
    break;
    case RED_COLOR:
    glColor3f(1, 0.0, 0.0);
    break;
    case YELLOW_COLOR:
    glColor3f(1, 1, 0.0);
    break;
    case BLACK_COLOR:
    glColor3f(0.0, 0.0, 0.0);
    break;
  }
}
void displayQuad(float x, float y){
  glBegin(GL_QUADS);
  displayVertex(x, y);
  glEnd();
}
void displayEdge(float x, float y){
  glBegin(GL_LINE_LOOP);
  displayVertex(x, y);
  glEnd();
}
void displayNumber(int number, float x, float y){
  char buffer[10];
  glRasterPos3f(x+CELL_SIDE_LENGTH/4, y+CELL_SIDE_LENGTH/3, 0);
  sprintf(buffer, "%d", number);
  for(int i = 0; buffer[i]!='\0'; i++)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buffer[i]);
}
void displayVertex(float x, float y){
  glVertex3f(x, y, 0);
  glVertex3f(x+CELL_SIDE_LENGTH, y, 0);
  glVertex3f(x+CELL_SIDE_LENGTH, y+CELL_SIDE_LENGTH, 0);
  glVertex3f(x, y+CELL_SIDE_LENGTH, 0);
}
void reshape(int width, int height){
  glutSetWindow(mainWindow);
  GLUI_Master.auto_set_viewport();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, (GLfloat)width / (GLfloat)height, 0.1, 100.0);
  glMatrixMode(GL_MODELVIEW);
}
int main(int argc, char **argv){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(800, 800);
  glutInitWindowPosition(100, 100);
  mainWindow = glutCreateWindow("Bookstore Proyect");
  glutDisplayFunc(display);
  glutSpecialFunc(special);
  GLUI_Master.set_glutReshapeFunc(reshape);
  GLUI_Master.set_glutIdleFunc(idle);
  GLUI* form = GLUI_Master.create_glui_subwindow(mainWindow, GLUI_SUBWINDOW_BOTTOM);
  form->set_main_gfx_window(mainWindow);
  form->add_spinner("ISBN", GLUI_SPINNER_INT, &book.isbn);
  form->add_edittext("Title", GLUI_EDITTEXT_TEXT, &book.title);
  form->add_column(false);
  form->add_edittext("Author", GLUI_EDITTEXT_TEXT, &book.author);
  form->add_edittext("Editorial", GLUI_EDITTEXT_TEXT, &book.editorial);
  form->add_column(true);
  form->add_button("Insert", INSERT_BOOK, button);
  form->add_button("Delete", DELETE_BOOK, button);
  form->add_column(false);
  form->add_checkbox("Show ", &search);
  form->add_button("Clear", CLEAR_FORM, button);
  form->add_column(true);
  form->add_statictext("Time Elapsed:");
  timer = form->add_statictext("0");
  form->add_button("Delete All", DELETE_ALL, button);
  glutMainLoop();
  bptDeleteAll(&bookstore);
}
