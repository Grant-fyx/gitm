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

//logs
void logs(){
    //首先读取次数
    FILE *file1=fopen("./.gitm/logs/count","r");
    int count;
    fread(&count,4,1,file1);
    fclose(file1);

    //然后打开logs文件夹读取日志
    file1=fopen("./.gitm/logs/logs","r");
    //读取count次
    char **c=malloc(count*(sizeof(char*)));
    int *merge=malloc(count*(sizeof(int)));
    char **MERGE=malloc(count*(sizeof(char*)));
    char **Date=malloc(count*(sizeof(char*)));
    char **MESSAGE=malloc(count*(sizeof(char*)));
    //循环读入
    for(int i=0;i<count;i++){
        //读取47个commit
        *(c+i)=malloc(48);
        memset(*(c+i),0,48);
        fread(*(c+i),1,47,file1);
        //读取flag
        fread((merge+i),4,1,file1);
        //如果不为零，就要读取
        if(*(merge+i)!=0){
            //待补充
        }
        //读取Date
        //读取长度
        int len;
        fread(&len,4,1,file1);
        *(Date+i)=malloc(len+1);
        memset(*(Date+i),0,len+1);
        fread(*(Date+i),1,len,file1);
        //读取消息
        //读取长度
        fread(&len,4,1,file1);
        *(MESSAGE+i)=malloc(len+1);
        memset(*(MESSAGE+i),0,len+1);
        fread(*(MESSAGE+i),1,len,file1);
    }
    //倒序输出
    for(int i=count-1;i>=0;i--){
        //打印commit
        printf("%s\n",*(c+i));
        //打印MERGE
        if(*(merge+i)!=0){
            //待补充
        }
        //打印Date
        printf("%s\n",*(Date+i));
        //打印MESSAGE
        printf("%s\n",*(MESSAGE+i));
        //换行
        printf("\n");
    }
}

//checkout
void checkout(char *hash){
    //检查工作区是否干净
    //如果有脏改动（待补充）
    //如果没有脏改动
        //用回调函数删除当前仓库中内容
        walk_path(".",path_delete, NULL);
        //回溯
            //以二进制只读模式打开对应版本的commit对象
            char *findpath=malloc(520);
            sprintf(findpath,"./.gitm/objects/commit/%s",hash);
            FILE *file1=fopen(findpath,"rb");
            //开始反序列化
                //读入提交消息
                int len;
                fread(&len,4,1,file1);
                char *message=malloc(len+1);
                fread(message,1,len,file1);
                //读入时间戳
                fread(&len,4,1,file1);
                char *Date=malloc(len+1);
                fread(Date,1,len,file1);
                //读入文件个数
                int NumOfFile;
                fread(&NumOfFile,4,1,file1);
                //处理NumOfFile个文件
                for(int i=0;i<NumOfFile;i++){
                    //读入原路径
                    fread(&len,4,1,file1);
                    char *destinationPath=malloc(len+1);
                    fread(destinationPath,1,len,file1);
                    //读入HASH
                    fread(&len,4,1,file1);
                    char *filehash=malloc(len+1);
                    fread(filehash,1,len,file1);
                    //读入文件名
                    fread(&len,4,1,file1);
                    char *FileName=malloc(len+1);
                    fread(FileName,1,len,file1);
                    //利用哈希值把文件复制到新路径
                    char *SourcePath=malloc(520);
                    sprintf(SourcePath,"./.gitm/objects/file/%s",filehash);
                    ERROR("<SourcePath:%s><destinationPath:%s>",SourcePath,destinationPath);
                    if(copy_file(SourcePath,destinationPath)){
                        ERROR("fail to copy file");
                    };
                free(message);
                free(Date);
                free(destinationPath);
                free(filehash);
                free(FileName);
                free(SourcePath);
                fclose(file1);
                }

    //更新HEAD指针
    FILE *file2=fopen("./.gitm/refs/head/head","wb");
    fwrite(hash,1,40,file2);
    fclose(file2);

}