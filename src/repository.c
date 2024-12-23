#include "repository.h"
#include "gitm.h"
#include "blob.h"
#include "commit.h"
#include "list.h"

//init
int init(){
    char const *path="./.gitm";
    //创建.gitm文件夹，创建失败即说明已经有仓库了，返回-1
    if(make_directory(path)){
        return -1;
    }

    //创建（对象）objects文件夹
    if(make_directory("./.gitm/objects")){
        return -1;
    };
        //在其下创建file子文件夹和commit子文件夹
        if(make_directory("./.gitm/objects/file")){
        return -1;
        };

        if(make_directory("./.gitm/objects/commit")){
        return -1;
        };


    //创建（日志）logs文件夹
    if(make_directory("./.gitm/logs")){
        return -1;
    };

    //创建refs文件夹，存放各种引用，如分支、标签等的指针文件
    if(make_directory("./.gitm/refs")){
        return -1;
    };
        //在其下创建head文件夹，存放指向分支指针
        if(make_directory("./.gitm/refs/head")){
            return -1;
        };

        //创建tags文件夹，存放指向分支指针（哈希值？）
        if(make_directory("./.gitm/refs/tags")){
            return -1;
        };\

    //创建（临时存放区）temp文件夹
    if(make_directory("./.gitm/temp")){
        return -1;
    };
    return 0;
}

//commit
void commit(char * message){
    //定义一个commit结构体并记录元数据
    CommitStruct STR;
    STR.message=malloc(520);
    STR.Time=malloc(520);
    //消息
    strcpy(STR.message,message);
    //时间戳
    timestamp(STR.Time);
    //定义一个链表
    struct LL list;
    list.head=NULL;
    list.tail=NULL;
    //处理文件对象
    walk_path(".", path_copy,&list);
    //将STR.ptr指向头指针
    STR.ptr=list.head;

    //将该commit结构体序列化，并计算其哈希值，即其提交号
    StoreCommit(&STR);

}