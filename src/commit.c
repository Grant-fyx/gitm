#include "commit.h"
#include "blob.h"
#include "gitm.h"
extern int NumOfFile;

void Write(char *s,FILE *file1,int *n){
    int len=strlen(s);
    fwrite(&len,4,1,file1);
    fwrite(s,1,len,file1);
    *n+=(len+4);
}


//将提交序列化
void StoreCommit(CommitStruct *CommitStruct){
    //计算文件大小
    int n=0;
    //在./.gitm/temp下创建一个文件tempcommit
    FILE *file1=fopen("./.gitm/temp/tempcommit","wb");
    //写入提交消息
    int len=strlen(CommitStruct->message);
    fwrite(&len,4,1,file1);
    fwrite(CommitStruct->message,1,len,file1);
    n+=(len+1);
    //写入时间戳
    len=strlen(CommitStruct->Time);
    fwrite(&len,4,1,file1);
    fwrite(CommitStruct->Time,1,len,file1);
    n+=(len+1);
    //写入文件个数
    fwrite(&NumOfFile,sizeof(int),1,file1);
    n+=sizeof(int);
    //写入链表中NumOfFile个文件对象结构体
    FileStruct *ptr=CommitStruct->ptr;
    while(ptr!=NULL){
        Write(ptr->FilePath,file1,&n);
        Write(ptr->HASH,file1,&n);
        Write(ptr->FileName,file1,&n);
        ptr=ptr->next;
    }
    //计算其哈希值
    char *CommitSum=malloc(41);
    memset(CommitSum,0,41);
    fclose(file1);
    file1=fopen("./.gitm/temp/tempcommit","rb");
    char *data=malloc(n+1);
    fread(data,1,n,file1);
    fclose(file1);
    if(sha1sum(CommitSum,data,n)){
        ERROR("fail to calculate the commit hash");
    }
    free(data);
    printf("%s\n",CommitSum);
    //将这个tempcommit改名为其哈希值，同时移入commit文件夹
        //构建路径
        char *newpath=malloc(520);
        sprintf(newpath,"./.gitm/objects/commit/%s",CommitSum);
        //把tempcommit移动过去并以哈希值命名
        if(rename_file("./.gitm/temp/tempcommit", newpath)){
            ERROR("fail to rename the tempcommit");
        };
    

    //写入父提交的哈希值
    //如果存在，则写入，并且更新
    if(exists("./.gitm/refs/head/head")){
        file1=fopen("./.gitm/refs/head/head","r");
        //追加模式
        FILE *file2=fopen(newpath,"a");
        char buffer[41];
        fread(buffer,1,40,file1);
        //写入：
        fwrite(buffer,1,40,file2);
        fclose(file1);
        fclose(file2);
        //更新
        file1=fopen("./.gitm/refs/head/head","wb");
        fwrite(CommitSum,1,40,file1);
        fclose(file1);
    }
    //如果不存在，则创建并写入当前提交哈希值,将当前提交的父提交哈希值记为全0
    else {
        file1=fopen("./.gitm/refs/head/head","wb");
        fwrite(CommitSum,1,40,file1);
        fclose(file1);
        FILE *file2=fopen(newpath,"a");
        char buffer[41]={0};
        fwrite(buffer,1,40,file2);
        fclose(file2);
    }

    //commit的时候录入logs
    file1=fopen("./.gitm/logs/logs","a");
    //c:commit;flagM:Merge;D:date;in:message
        //commit
    char *s=malloc(50);
    sprintf(s,"commit: %s",CommitSum);
    fwrite(s,1,48,file1);
    free(s);
        //merge:如果flag为0，说明没有
    int flagM=0;
    fwrite(&flagM,4,1,file1);
        //Date:
    len=strlen(CommitStruct->Time)+6;
    fwrite(&len,4,1,file1);
    char *D=malloc(len+1);
    sprintf(D,"Date: %s",CommitStruct->Time);
    fwrite(D,1,len,file1);
    free(D);
        //message:
    len=strlen(CommitStruct->message);
    fwrite(&len,4,1,file1);
    fwrite(CommitStruct->message,1,len,file1);
    fclose(file1);
    //为logs计数
    file1=fopen("./.gitm/logs/count","a+");
    int temp;
    if(fread(&temp,4,1,file1)==0){
        temp=1;
        fwrite(&temp,4,1,file1);
    }
    else {
        temp++;
        fclose(file1);
        file1=fopen("./.gitm/logs/count","w");
        fwrite(&temp,4,1,file1);
        fclose(file1);
    }

}