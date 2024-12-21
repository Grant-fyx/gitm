#include "repository.h"
#include "gitm.h"
#include "blob.h"

//init
int init(){
    char const *path="./.gitm";
    //创建.gitm文件夹，创建失败即说明已经有仓库了，返回-1
    if(make_directory(path)){
        return -1;
    }

    //创建（对象）objects文件
    if(make_directory("./.gitm/objects")){
        return -1;
    };

    //创建（日志）logs文件
    if(make_directory("./.gitm/logs")){
        return -1;
    };

    //创建refs文件，存放各种引用，如分支、标签等的指针文件
    if(make_directory("./.gitm/refs")){
        return -1;
    };
        //在其下创建head文件，存放指向分支指针
        if(make_directory("./.gitm/refs/head")){
            return -1;
        };

        //创建tags文件，存放指向分支指针（哈希值？）
        if(make_directory("./.gitm/refs/tags")){
            return -1;
        };\
    return 0;
}

//commit
void commit(){
    walk_path(".", path_printer, "FYX");
}