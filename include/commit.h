#ifndef COMMIT_H
#define COMMIT_H

#include "blob.h"
typedef struct B{
    char *Time;
    char *message;
    FileStruct *ptr;
}CommitStruct;

//将提交序列化
void StoreCommit(CommitStruct *CommitStruct);



#endif