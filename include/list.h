#include<stdio.h>
#include <stdlib.h>
#include "blob.h"
//节点：
//头尾
struct LL{
    FileStruct * head;
    FileStruct *tail;
};//维护链表的头和尾

//创建节点
FileStruct *initnode(const char *path,char *hash,char *name);

//添加节点
void appendlist(struct LL *list, FileStruct *newp);