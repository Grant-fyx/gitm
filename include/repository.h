#ifndef REP_H
#define REP_H


#include<stdio.h>

//init
int init();

//commit
void commit(char * message);
void logs();

//checkout
int checkout(char *hash);

//merge
int merge(char *targethash);

#endif