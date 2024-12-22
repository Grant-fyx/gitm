#include "list.h"
#include<string.h>
//创建节点
FileStruct *initnode(const char *path,char *hash,char *name){
    //为这个新节点分配一个空间
    FileStruct *p=malloc(sizeof(*p));
    //存入数据
    p->FilePath=malloc(strlen(path));
    p->HASH=malloc(strlen(hash));
    p->FileName=malloc(strlen(name));
    strcpy(p->FilePath,path);
    strcpy(p->HASH,hash);
    strcpy(p->FileName,name);
    p->next=NULL;
    return p;
}

//添加节点
void appendlist(struct LL *list, FileStruct *newp){
    //如果链表为空
    if(list->head==NULL&&list->tail==NULL){
        list->head=newp;
        list->tail=newp;
    }
    //不为空
    //将原节点末尾的next指向新节点
    else {
        list->tail->next=newp;
        list->tail=newp;
    }
}