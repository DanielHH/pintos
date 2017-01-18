
#include <stdio.h>
#include <stdlib.h>

struct list_item {
  int value;
  struct list_item * next;
};

void append(struct list_item *first, int x) {
  struct list_item *new_item = (struct list_item *)malloc (sizeof (struct list_item));
  new_item->value = x;
  new_item->next = NULL;
  struct list_item *current = first;
  while (current->next != NULL) {
    current = current->next;
  }
  current->next = new_item;
};


void prepend(struct list_item *first, int x) {
  struct list_item *new_item = (struct list_item *)malloc (sizeof (struct list_item));
  new_item->value = x;
  new_item->next = first->next;
  first->next = new_item;
};


void print(struct list_item *first) {
  struct list_item *current = first->next;
  while (current != NULL) {
    printf("%i ", current->value);
    current = current->next;
  }
  printf("\n");
};
    

void input_sorted(struct list_item *first, int x) {
  struct list_item *current = first;
  struct list_item *new_item = (struct list_item *)malloc (sizeof (struct list_item));
  while (current->next != NULL && x >= current->next->value) {
    current = current->next;
  }
  new_item->value = x;
  new_item->next = current->next;
  current->next = new_item;
};
  
void clear(struct list_item *first) {
  struct list_item *current = first->next;
  struct list_item *temp;
  while(current != NULL) {
    temp = current->next;
    free(current);
    current = temp;
  }
  first->next = NULL;
};

int main(int argc, char ** argv) {
  struct list_item root;
  root.value = -1; 
  root.next = NULL;
  append(&root, 9);
  append(&root, 7);
  prepend(&root, 4);
  input_sorted(&root, 6);
  input_sorted(&root, 2);
  input_sorted(&root, 3);
  print(&root);
  clear(&root);
  print(&root);
}
  
  
