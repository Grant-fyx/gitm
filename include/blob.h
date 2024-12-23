#ifndef BLOB_H
#define BLOB_H

#include "gitm.h"

//声明函数，对遍历到的每一个路径创建blob
void *path_copy(char const *path, void *arg);
//声明文件对象结构体
struct A{
    char *FilePath;
    char *HASH;
    char *FileName;
    struct A* next;
};
typedef struct A FileStruct;
//获取文件名的函数
void GetFilename(char *name,const char *path);
//删除文件的函数
void*path_delete(char const *path, void *arg);

#endif