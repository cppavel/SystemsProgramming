#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// include the header files with the declarations of listset
#include "listset.h"


struct listnode* listnode_new(char * item){
    struct listnode* ans = malloc(sizeof(struct listnode));
    ans->str = item;
    return ans;

}

// create a new, empty linked list set
struct listset * listset_new() {
    struct listset* ans = malloc(sizeof(struct listset));
    return ans;
}

/* check to see if an item is in the set
   returns 1 if in the set, 0 if not */
int listset_lookup(struct listset * this, char * item) {

    struct listnode* current = this->head;

    while(current!=NULL)
    {
        if(strcmp(current->str,item) == 0)
        {
            return 1;
        }
        current = current->next;
    }

    return 0;
}

// add an item, with number 'item' to the set
// has no effect if the item is already in the set.
// New items that are not already in the set should
// be added to the start of the list
void listset_add(struct listset * this, char * item) {

    if(!listset_lookup(this,item))
    {
        struct listnode * head = this->head;
        struct listnode * new_head = listnode_new(item);
        new_head->next = head;
        this->head = new_head;
    }

}

// remove an item with number 'item' from the set
void listset_remove(struct listset * this, char * item) {
    struct listnode* current = this->head;
    struct listnode* prev = this->head;
    while(current != NULL)
    {
        if(strcmp(current->str,item)==0)
        {
            if(current == this->head)
            {
                this->head = (this->head)->next;
            }
            else
            {
                prev->next = current->next;
            }
            free(current);
            break;
        }
        prev = current;
        current = current->next;
    }
}
  
// place the union of src1 and src2 into dest
void listset_union(struct listset * dest, struct listset * src1,
		   struct listset * src2) {

    struct listnode* current = src1->head;
    while(current!=NULL)
    {
        listset_add(dest,current->str);
        current = current->next;
    }

    current = src2->head;
    while(current!=NULL)
    {
        listset_add(dest,current->str);
        current = current->next;
    }
}

// place the intersection of src1 and src2 into dest
void listset_intersect(struct listset * dest, struct listset * src1,
		       struct listset * src2) {

    struct listnode* current = src1->head;
    while(current!=NULL)
    {
        if(listset_lookup(src2,current->str))
        {
            listset_add(dest, current->str);
        }
        current = current->next;
    }

}

// return the number of items in the listset
int listset_cardinality(struct listset * this) {

    int count = 0;
    struct listnode* current = this->head;

    while(current != NULL)
    {
        current = current->next;
        count = count + 1;
    }

    return count;
}

// print the elements of the list set
void listset_print(struct listset * this) {
  struct listnode * p;

  for ( p = this->head; p != NULL; p = p->next ) {
    printf("%s, ", p->str);
  }
  printf("\n");
}
