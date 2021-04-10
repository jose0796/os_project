#include<stdio.h>
#include "hash.h"

void main(){
    // str2IntHashTable * htable = (str2IntHashTable *) malloc(sizeof(str2IntHashTable)); 
    str2IntHashTable htable;

    initializeTable(&htable);

    addStr2IntNode(&htable, "carlos es marico", 22); 
    addStr2IntNode(&htable, "carlos es marico", 15); 

    addStr2IntNode(&htable, "hello", 15);
    printTable(htable);
}