#include "gitm.h"
#include "blob.h"
#include "list.h"

    //定义结构体包含文件个数和哈希值数组
    struct B{
        char **working_directory;
        int NumOfWorkingFile;
    };

//文件个数
int NumOfFile=0;
//获取文件名
void GetFilename(char *name,const char *path){
    char *ptr=strchr(path,'/');
    strcpy(name,ptr+1);
    return ;
}

int compare1(const void *a,const void *b){
    char **x=(char **)a;
    char **y=(char **)b;
    return strcmp(*x,*y);
}

//实现函数，对遍历到的每一个路径创建blob
    void *path_copy(char const *path, void *arg) {

        struct LL*list=(struct LL*)arg;
    //当读到的路径不是./gitm.exe时
    if(strcmp(path,"./gitm.exe")){
        NumOfFile++;
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
        memset(sum,0,41);
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
        //获取该文件的名字
        char *name=malloc(strlen(path));
        GetFilename(name,path);
        //利用链表记录该文件对象的信息
        appendlist(list,initnode(path,sum,name));
        //利用该哈希值查找objects/file里面有无该文件
            //构建路径
            char *findpath=malloc(520);
            memset(findpath,0,520);
            sprintf(findpath,"./.gitm/objects/file/%s",sum);
            //用bool exists查找该文件是否已经存在
                //如果找到：
                if(exists(findpath)){           
                    //删除tempfile
                    if(remove_file("./.gitm/temp/tempfile")){
                        ERROR("fail to remove tempfile");
                    }
                }
                //如果没找到：
                else{
                    //把tempfile移动过去并以哈希值命名
                    if(rename_file("./.gitm/temp/tempfile",findpath)){
                        ERROR("fail to rename the tempfile");
                    }
                }
    }
    return NULL;
    }

//实现函数。删除当前路径下的文件
    void *path_delete(char const *path, void *arg){
        //当读到的路径不是./gitm.exe时
        if(strcmp(path,"./gitm.exe")){
            //如果成功
            if(remove_file(path)){
                ERROR("fail to delete the file '%s' in working directory\n",path);
            }
        }
        return NULL;
    }


//读入文件对象的信息
void Read(char *s,FILE *file1,int *n){
    int len=strlen(s);
    fread(&len,4,1,file1);
    fwrite(s,1,len,file1);
    *n+=(len+4);
}

//实现函数，获取文件里的内容
    void *acquire(char const *path, void *arg){
        struct B*working=(struct B*)arg;
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
            *(working->working_directory+working->NumOfWorkingFile)=malloc(41);
            memset(*(working->working_directory+working->NumOfWorkingFile),0,41);
            char *data=malloc(count+1);
            fclose(file1);
            fclose(file2);
            file1=fopen(path,"rb");
            fread(data,1,count,file1);
            fclose(file1);
            if(sha1sum(*(working->working_directory+working->NumOfWorkingFile),data,count)){
                ERROR("fail to calculate the hash");
            };
            free(data);
            working->NumOfWorkingFile++;
            if(remove_file("./.gitm/temp/tempfile")) {
                ERROR("fail to delete");
            }
        }
        return NULL;
    }

//检查是否有脏改动
int dirty(){
    //定义字符串数指针组来记录当前工作区里的文件的哈希值
    struct B working;
    working.working_directory=malloc(520*8);
    working.NumOfWorkingFile=0;
    //利用回调函数获取工作区的哈希值数组
    walk_path(".",acquire,&working);
    //找到当前头指针,获取当前提交的哈希值
    char *head=malloc(41);
    memset(head,0,41);
    FILE *headfile=fopen("./.gitm/refs/head/head","rb");
    fread(head,1,40,headfile);
    fclose(headfile);
    //以二进制只读模式打开对应版本的commit对象
    char *findpath=malloc(520);
    sprintf(findpath,"./.gitm/objects/commit/%s",head);
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
        char **headhash=malloc(NumOfFile*sizeof(char *));
        //处理NumOfFile个文件
        for(int i=0;i<NumOfFile;i++){
            //读入原路径
            fread(&len,4,1,file1);
            char *destinationPath=malloc(len+1);
            fread(destinationPath,1,len,file1);
            *(destinationPath+len)='\0';
            //读入HASH
            fread(&len,4,1,file1);
            fread(*(headhash+i),1,len,file1);
            *(*(headhash+i)+len)='\0';
            //读入文件名
            fread(&len,4,1,file1);
            char *FileName=malloc(len+1);
            fread(FileName,1,len,file1);

            free(destinationPath);
            free(FileName);
        }
        fclose(file1);
        free(message);
        free(Date);

    if(working.NumOfWorkingFile!=NumOfFile){
        ERROR("num difference");
        return -1;
    }
    else {
        //比较
        qsort(working.working_directory,NumOfFile,8,compare1);
        qsort(headhash,NumOfFile,8,compare1);
        for(int j=0;j<NumOfFile;j++){
            if(strcmp(*(headhash+j),*(working.working_directory+j))){
                ERROR("content difference");
                return -1;
            }
        }
        return 0;
    }
}