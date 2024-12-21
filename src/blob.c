#include "gitm.h"
#include "blob.h"

//定义函数，对遍历到的每一个路径复制一个副本
    //（测试：打印所有路径）
    void *path_printer(char const *path, void *arg) {
    char const *name = (char *)arg;
    printf("%s from %s\n", path, name);
    return NULL;
    }