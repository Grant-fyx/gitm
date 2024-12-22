#include "gitm.h"
#include "blob.h"
#include "list.h"
//获取文件名
void GetFilename(char *name,const char *path){
    char *ptr=strchr(path,'/');
    strcpy(name,ptr+1);
    return ;
}



//实现函数，对遍历到的每一个路径创建blob
    void *path_copy(char const *path, void *arg) {
        struct LL*list=(struct LL*)arg;
    //当读到的路径不是./gitm.exe时
    if(strcmp(path,"./gitm.exe")){
        //以二进制打开该文件
        FILE *file1=fopen(path,"rb");
        //在./.gitm/temp下创建一个文件tempfile并复制过去
        FILE *file2=fopen("./.gitm/temp/tempfile","wb");
        char buffer[1024];
        int n;
        int count=0;//定义count计数来计算该文件有多少内存，从而为他分配空间，从而计算哈希值
        while((n=fread(buffer,1,1024,file1))!=0){
            fwrite(buffer,1,n,file2);
            count+=n;
        }
        //计算该文件哈希值
        char *sum=malloc(41);
        char *data=malloc(count+1);
        fclose(file1);
        fclose(file2);
        file1=fopen(path,"rb");
        fread(data,1,count,file1);
        fclose(file1);
        if(sha1sum(sum,data,count)){
            ERROR("fail to calculate the hash");
        };
        free(data);
        printf("%s",sum);
        //获取该文件的名字
        char *name=malloc(20);
        GetFilename(name,path);
        //利用链表记录该文件对象的信息
        appendlist(list,initnode(path,sum,name));
        //利用该哈希值查找objects/file里面有无该文件
            //构建路径
            char *findpath=malloc(520);
            sprintf(findpath,"./.gitm/objects/file/%s",sum);
            //用bool exists查找该文件是否已经存在
                //如果找到：
                if(exists(findpath)){        
                    printf("find\n");    
                    //删除tempfile
                    if(remove_file("./.gitm/temp/tempfile")){
                        ERROR("fail to remove tempfile");
                    }
                }
                //如果没找到：
                else{
                    printf("not find\n");
                    //把tempfile移动过去并以哈希值命名
                    if(rename_file("./.gitm/temp/tempfile",findpath)){
                        ERROR("fail to rename the tempfile");
                    }
                    else {
                        printf("success:%s\n",findpath);
                    }
                }
    }

    return NULL;
    }