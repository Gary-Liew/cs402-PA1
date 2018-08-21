//
//  my402list.c
//  PA1 code
//
//  Created by YING LIU on 2017/1/15.
//  Copyright © 2017年 YING LIU. All rights reserved.
//

#include "my402list.h"
#include "cs402.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//get the length of the current list
int My402ListLength(My402List* mylist){
   return mylist->num_members;
}

//decide if the list is empty or not and return 0 or 1
int  My402ListEmpty(My402List* mylist){
    if(mylist->num_members<=0){
        return TRUE;
    }
    else{
        return FALSE;
    }
}

//append the element at the last location of the list and return 0 or 1
int  My402ListAppend(My402List* mylist, void* object){
    My402ListElem *node =(struct tagMy402ListElem *)malloc(sizeof(struct tagMy402ListElem));
    node->obj=object;
    if(mylist->num_members<=0){
        (*mylist).anchor.next=node;
        (*node).next=&((*mylist).anchor);
        (*mylist).anchor.prev=node;
        (*node).prev=&((*mylist).anchor);
        mylist->num_members++;
    }
    else{
        My402ListElem *last =My402ListLast(mylist);
        (*last).next=node;
        (*node).next=&((*mylist).anchor);
        (*mylist).anchor.prev=node;
        (*node).prev=last;
        mylist->num_members++;
    }
    return TRUE;
}

//prepend the element at the first location of the list and return 0 or 1
int  My402ListPrepend(My402List* mylist, void* object){
    My402ListElem *node =(struct tagMy402ListElem *)malloc(sizeof(struct tagMy402ListElem));
    node->obj=object;
    if(mylist->num_members<=0){
        (*mylist).anchor.next=node;
        (*node).next=&((*mylist).anchor);
        (*mylist).anchor.prev=node;
        (*node).prev=&((*mylist).anchor);
        mylist->num_members++;
    }
    else{
        My402ListElem *first =My402ListFirst(mylist);
        (*mylist).anchor.next=node;
        (*node).next=first;
        (*first).prev=node;
        (*node).prev=&((*mylist).anchor);
        mylist->num_members++;
        
    }
    return 1;
}

//unlink one element from the list
void My402ListUnlink(My402List* mylist, My402ListElem* element){
    if(mylist->num_members>0){
    mylist->num_members--;
    ((*element).next)->prev=(*element).prev;
    ((*element).prev)->next=(*element).next;
    free(element);
    }
    else{
        return;
    }
}

//unlink and delete all elements from the list
void My402ListUnlinkAll(My402List* mylist){
    if(mylist->num_members>0){
        My402ListElem *deletion =My402ListLast(mylist);
        while(deletion){
            My402ListElem *nextdel =deletion;
            mylist->num_members=mylist->num_members-1;
            deletion=My402ListPrev(mylist, deletion);
            My402ListUnlink(mylist, nextdel);
            
        }
    }
    else{
        return;
    }
    
}

//Insert one node between element and element->prev
int My402ListInsertBefore(My402List* mylist, void* object, My402ListElem* element){
    if(element==NULL){
       return My402ListPrepend(mylist,object);
    }
    else{
         My402ListElem *node =(struct tagMy402ListElem *)malloc(sizeof(struct tagMy402ListElem));
        node->obj=object;
        My402ListElem* previous=element->prev;
        (*(element->prev)).next=node;
        node->next=element;
        element->prev=node;
        node->prev=previous;
        mylist->num_members++;
         return 1;
    }
    
}

//Insert one node between element and element->next
int My402ListInsertAfter(My402List* mylist,void *object, My402ListElem *element){
    if(element==NULL){
        return My402ListAppend(mylist,object);
    }
    else{
        My402ListElem *node =(struct tagMy402ListElem *)malloc(sizeof(struct tagMy402ListElem));
        node->obj=object;
        My402ListElem* nextnode=element->next;
        element->next=node;
        node->next=nextnode;
        nextnode->prev=node;
        node->prev=element;
        mylist->num_members++;
        return TRUE;
    }
}

//return the first node of the list
My402ListElem *My402ListFirst(My402List* mylist){
    if(mylist->num_members>0){
        return ((*mylist).anchor).next;
    }
    else{
        return NULL;
    }
}

//return the last node of the list
My402ListElem *My402ListLast(My402List* mylist){
    if(mylist->num_members>0){
        return ((*mylist).anchor).prev;
    }
    else{
        return NULL;
    }

}

//return the next node of the current element node
My402ListElem *My402ListNext(My402List* mylist, My402ListElem* element){
    if(element==(mylist->anchor).prev){
        return NULL;
    }
    else{
        return element->next;
    }
}

//return the previous node of the current element node
My402ListElem *My402ListPrev(My402List* mylist, My402ListElem* element){
    if(element==(mylist->anchor).next){
        return NULL;
    }
    else{
        return element->prev;
    }
}

//Find the element in the list
My402ListElem *My402ListFind(My402List* mylist, void* object){
    My402ListElem * last = My402ListLast(mylist);
    while(last){
        if(last->obj==object){
            return last;
            break;
        }
        else{
            last=last->prev;
        }
    }
    return NULL;
}

//Initialize the list into an empty list
int My402ListInit(My402List* mylist){
    ((*mylist).anchor).next = &((*mylist).anchor);
    ((*mylist).anchor).prev = &((*mylist).anchor);
    ((*mylist).anchor).obj = NULL;
    mylist -> num_members = 0;
    return 1;
}















