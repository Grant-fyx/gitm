#include "commit.h"
#include "blob.h"
#include "gitm.h"
extern int NumOfFile;
//将提交序列化
void StoreCommit(CommitStruct *CommitStruct){
    //计算文件大小
    int n=0;
    //在./.gitm/temp下创建一个文件tempcommit
    FILE *file1=fopen("./.gitm/temp/tempcommit","wb");
    //写入提交消息
    fwrite(CommitStruct->message,1,strlen(CommitStruct->message),file1);
    n+=strlen(CommitStruct->message);
    //写入时间戳
    fwrite(CommitStruct->Time,1,strlen(CommitStruct->Time),file1);
    n+=strlen(CommitStruct->Time);
    //写入文件个数
    fwrite(&NumOfFile,sizeof(int),1,file1);
    n+=sizeof(int);
    //写入链表中NumOfFile个文件对象结构体
    FileStruct *ptr=CommitStruct->ptr;
    while(ptr!=NULL){
        fwrite(ptr,sizeof(FileStruct),1,file1);
        n+=sizeof(FileStruct);
        ptr=ptr->next;
    }
    n+=NumOfFile*sizeof(FileStruct);
    //计算其哈希值
    char *CommitSum=malloc(41);
    fclose(file1);
    file1=fopen("./.gitm/temp/tempcommit","rb");
    char *data=malloc(n+1);
    //消息
    fread(data,1,strlen(CommitStruct->message),file1);
    //时间戳
    fread(data,1,strlen(CommitStruct->Time),file1);
    //文件个数
    fread(data,sizeof(int),1,file1);
    //NumOfFile个文件对象结构体
    ptr=CommitStruct->ptr;
    while(ptr!=NULL){
        fread(data,sizeof(FileStruct),1,file1);
        ptr=ptr->next;
    }
    fclose(file1);
    if(sha1sum(CommitSum,data,n)){
        ERROR("fail to calculate the commit hash");
    }
    free(data);
    printf("commit:%s\n",CommitSum);
    //将这个tempcommit改名为其哈希值，同时移入commit文件夹
        //构建路径
        char *newpath=malloc(520);
        sprintf(newpath,"./.gitm/objects/commit/%s",CommitSum);
        //把tempcommit移动过去并以哈希值命名
        if(rename_file("./.gitm/temp/tempcommit", newpath)){
            ERROR("fail to rename the tempcommit");
        };
}