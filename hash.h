#include<stdlib.h>
#include<string.h>
#include<stdio.h>

#define MAX_TABLE_SIZE 40

typedef struct _str2IntHashNode {
    int data; 
    char key[20]; 
    struct _str2IntHashNode * next; 
} str2int_node; 

typedef struct _str2IntHashTable { 
    struct _str2IntHashNode * innerTable[40];
} str2IntHashTable; 

int getHash(char * str){
    int sum = 0; 
    int i = 0; 
    while(str[i] != '\0'){
        sum += str[i]; 
        ++i; 
    }
    return sum%MAX_TABLE_SIZE; 
}

int existsKey(str2IntHashTable table, char * key){
    int hashIndex = getHash(key); 

    str2int_node * curr = table.innerTable[hashIndex]; 
    str2int_node * prev = NULL; 

    
    while (curr != NULL)
    {
        if (!strcmp(curr->key, key)){
            return 1; 
        }

        prev = curr; 
        curr = curr->next; 
    }

    

    return 0; 
    
    
}

void initializeTable(str2IntHashTable * table){

    for(int i = 0 ; i < MAX_TABLE_SIZE; ++i){
       table->innerTable[i] = NULL; 
    }

}
void addStr2IntNode(str2IntHashTable * table, char * key, int value){
    
    int hashIndex = getHash(key); 


    if (table->innerTable[hashIndex] == NULL){
        table->innerTable[hashIndex] = (str2int_node * )malloc(sizeof(str2int_node)); 
        strcpy(table->innerTable[hashIndex]->key, key); 
        table->innerTable[hashIndex]->data = value; 
        table->innerTable[hashIndex]->next = NULL; 
        return; 
    }

    str2int_node * curr = table->innerTable[hashIndex]; 
    str2int_node * prev = NULL; 

    while(curr != NULL){

        if (!strcmp(curr->key, key)){
            curr->data = value; 
            return; 
        }

        prev = curr;
        curr = curr->next; 
    }

    str2int_node * newNode = (str2int_node * ) malloc(sizeof(str2int_node)); 
    prev->next = newNode; 
    strcpy(newNode->key, key); 
    newNode->data = value; 
    newNode->next = NULL; 
    
}


int getValue(str2IntHashTable table, char * key){
    int hashIndex = getHash(key); 
    if (table.innerTable[hashIndex] == NULL){
        return __INT_MAX__; 
    }


    str2int_node * curr = table.innerTable[hashIndex]; 
    str2int_node * prev = NULL; 
    if (!strcmp(curr->key, key)){
        return curr->data; 
    }

    while(curr != NULL){

        if (!strcmp(key, curr->key)){
            return curr->data; 
        }

        prev = curr; 
        curr = curr->next;
    }

    return __INT_MAX__; 



}

void printTable(str2IntHashTable table){
    for(int i = 0; i < MAX_TABLE_SIZE; ++i){
        if (table.innerTable[i] != NULL){
            str2int_node * curr = table.innerTable[i]; 
            str2int_node * prev = NULL; 
            while(curr != NULL){
                printf("{ %s : %d } -> ", curr->key, curr->data); 
                prev = curr; 
                curr = curr->next; 
            }

            printf("NULL \n"); 
        }
    }
}
