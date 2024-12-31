#include "repository.h"
#include "gitm.h"
#include "blob.h"
#include "commit.h"
#include "list.h"

extern int NumOfFile;

//定义全局变量存入祖先数组
int count=0;
//定义全局变量存入公共祖先数组
int countofancestor=0;
//递归函数寻找祖先
void FindAncestor(char *hash,char **AncestorOfTarget){
    //有了当前节点的哈希值
    //打开该文件并开始反序列化
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
            *(destinationPath+len)='\0';
            //读入HASH
            fread(&len,4,1,file1);
            char *filehash=malloc(len+1);
            fread(filehash,1,len,file1);
            *(filehash+len)='\0';
            //读入文件名
            fread(&len,4,1,file1);
            char *FileName=malloc(len+1);
            fread(FileName,1,len,file1);
            free(destinationPath);
            free(filehash);
            free(FileName);
        }
        free(message);
        free(Date);
        //读取两个哈希值
        char zero[40]={0};
            //第一个
        char *hash1=malloc(41);
        memset(hash1,0,41);
        fread(hash1,1,40,file1);
        //如果不为零，并且原先公共祖先数组中没有该哈希值，那么将其放进公共祖先数组里,并且找寻他的公共祖先
        //循环遍历公共祖先数组
        int flag =1;
        for(int k=0;k<count;k++){
            if(!strcmp(*(AncestorOfTarget+k),hash1)){
                flag=0;
                break;
            }
        }
        if(strcmp(hash1,zero)&&flag){
            *(AncestorOfTarget+count)=malloc(41);
            *(*(AncestorOfTarget+count)+40)='\0';
            strcpy(*(AncestorOfTarget+count),hash1);
            count++;
            //递归寻找该提交的公共祖先
            FindAncestor(hash1,AncestorOfTarget);
        }

            //第二个
        char *hash2=malloc(41);
        memset(hash2,0,41);
        fread(hash2,1,40,file1);
        //如果不为零，并且原先公共祖先数组中没有该哈希值，那么将其放进公共祖先数组里,并且找寻他的公共祖先
        //循环遍历公共祖先数组
        flag =1;
        for(int k=0;k<count;k++){
            if(!strcmp(*(AncestorOfTarget+k),hash2)){
                flag=0;
                break;
            }
        }
        if(strcmp(hash2,zero)&&flag){
            *(AncestorOfTarget+count)=malloc(41);
            *(*(AncestorOfTarget+count)+40)='\0';
            strcpy(*(AncestorOfTarget+count),hash2);
            count++;
            //递归寻找该提交的公共祖先
            FindAncestor(hash2,AncestorOfTarget);
        }
    return ;
}

//递归函数回溯head提交
void tracing(char *hash,char **AncestorOfTarget,char **commonancestor){
    //有了当前节点的哈希值
    //打开该文件并开始反序列化
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
            *(destinationPath+len)='\0';
            //读入HASH
            fread(&len,4,1,file1);
            char *filehash=malloc(len+1);
            fread(filehash,1,len,file1);
            *(filehash+len)='\0';
            //读入文件名
            fread(&len,4,1,file1);
            char *FileName=malloc(len+1);
            fread(FileName,1,len,file1);
            free(destinationPath);
            free(filehash);
            free(FileName);
        }
        free(message);
        free(Date);
    //先比较自己
    //遍历祖先数组
    for(int k=0;k<count;k++){
        if(!strcmp(*(AncestorOfTarget+k),hash)){
            *(commonancestor+countofancestor)=malloc(41);
            *(*(commonancestor+countofancestor)+40)='\0';
            strcpy(*(commonancestor+countofancestor),hash);
            countofancestor++;
            return;
        }
    }
    //读取两个哈希值
    //第一个
        char *hash1=malloc(41);
        memset(hash1,0,41);
        fread(hash1,1,40,file1);
        int flag=0;
        //如果不为零，则与祖先数组比较
        if(strlen(hash1)!=0){
            //遍历祖先数组
            for(int k=0;k<count;k++){
                //如果找到了，存入commonancestor
                if(!strcmp(*(AncestorOfTarget+k),hash1)){
                    *(commonancestor+countofancestor)=malloc(41);
                    *(*(commonancestor+countofancestor)+40)='\0';
                    strcpy(*(commonancestor+countofancestor),hash1);
                    countofancestor++;
                    flag=1;
                    break;
                }
            }
            //如果flag为零，说明没找到，则递归继续寻找
            if(flag==0){
                tracing(hash1,AncestorOfTarget,commonancestor);
            }
        }

    //第二个
    char *hash2=malloc(41);
    memset(hash2,0,41);
    fread(hash2,1,40,file1);
    flag=0;
    //如果不为零，则与祖先数组比较
    if(strlen(hash2)!=0){
        //遍历祖先数组
        for(int k=0;k<count;k++){
            //如果找到了，存入commonancestor
            if(!strcmp(*(AncestorOfTarget+k),hash2)){
                *(commonancestor+countofancestor)=malloc(41);
                *(*(commonancestor+countofancestor)+40)='\0';
                strcpy(*(commonancestor+countofancestor),hash2);
                countofancestor++;
                flag=1;
                break;
            }
        }
        //如果flag为零，说明没找到，则递归继续寻找
        if(flag==0){
            tracing(hash2,AncestorOfTarget,commonancestor);
        }
    }
    return ;
}

//递归函数回溯记录步数
int steps(char *s,char *hash){
    int step=-1;
    //先比较自己（即最基本的终止条件）
    if(!strcmp(s,hash)){
        step=0;  
        return step;
    }

//有了当前节点的哈希值
    //打开该文件并开始反序列化
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
            *(destinationPath+len)='\0';
            //读入HASH
            fread(&len,4,1,file1);
            char *filehash=malloc(len+1);
            fread(filehash,1,len,file1);
            *(filehash+len)='\0';
            //读入文件名
            fread(&len,4,1,file1);
            char *FileName=malloc(len+1);
            fread(FileName,1,len,file1);
            free(destinationPath);
            free(filehash);
            free(FileName);
        }
        free(message);
        free(Date);    

    //读取两个哈希值
    //第一个
        char *hash1=malloc(41);
        memset(hash1,0,41);
        fread(hash1,1,40,file1);
        int step1;
        //如果不为零,则继续往下找
        if(strlen(hash1)!=0){
            step1=steps(s,hash1);
        }
        //否则返回-1
        else {
            step1=-1;
        }
    //第二个
        char *hash2=malloc(41);
        memset(hash2,0,41);
        fread(hash2,1,40,file1);
        fclose(file1);
        int step2;
        //如果不为零,则继续往下找
        if(strlen(hash2)!=0){
            step2=steps(s,hash2);
        }
        //否则返回-1
        else {
            step2=-1;
        }
        //判断最终返回值
        if((step1==-1)&&(step2==-1)){
            return -1;
        }
        else if((step1==-1)&&(step2!=-1)){
            return (step2+1);
        }
        else if ((step1!=-1)&&(step2==-1)){
            return (step1+1);
        }
        else {
            return ((step1<step2?step1:step2)+1);
        }
}

//获取LCA,target,head所对应文件,返回值是文件个数
//定义结构体
typedef struct G{
    char *name;
    char *hash;
}Getfile;
int getfile(Getfile **arr,char *hash){
    //有了当前节点的哈希值
    //打开该文件并开始反序列化
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
            *(destinationPath+len)='\0';

            //为结构体分配空间
            *(arr+i)=malloc(sizeof(Getfile));

            //读入HASH
            fread(&len,4,1,file1);
            (*(arr+i))->hash=malloc(len+1);
            fread((*(arr+i))->hash,1,len,file1);
            *((*(arr+i))->hash+len)='\0';
            //读入文件名
            fread(&len,4,1,file1);
            (*(arr+i))->name=malloc(len+1);
            fread((*(arr+i))->name,1,len,file1);
            *((*(arr+i))->name+len)='\0';
            free(destinationPath);
        }
        free(message);
        free(Date);
        fclose(file1);
        return NumOfFile;
}


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

    //获取头指针
        char *head=malloc(41);
        memset(head,0,41);
        FILE *headfile=fopen("./.gitm/refs/head/head","rb");
        fread(head,1,40,headfile);
        fclose(headfile);
    //开始打印日志
        while(strlen(head)!=0){
            //打开该文件
            char *findpath=malloc(520);
            sprintf(findpath,"./.gitm/objects/commit/%s",head);
            FILE *file1=fopen(findpath,"rb");
            //开始反序列化
                //读入提交消息
                int len;
                fread(&len,4,1,file1);
                char *message=malloc(len+1);
                memset(message,0,len+1);
                fread(message,1,len,file1);
                //读入时间戳
                fread(&len,4,1,file1);
                char *Date=malloc(len+1);
                memset(Date,0,len+1);
                fread(Date,1,len,file1);
                len=strlen(Date)+6;
                char *D=malloc(len+1);
                sprintf(D,"Date: %s",Date);
                //读入文件个数
                int NumOfFile;
                fread(&NumOfFile,4,1,file1);
                //处理NumOfFile个文件
                for(int i=0;i<NumOfFile;i++){
                    //读入原路径
                    fread(&len,4,1,file1);
                    char *destinationPath=malloc(len+1);
                    fread(destinationPath,1,len,file1);
                    *(destinationPath+len)='\0';
                    //读入HASH
                    fread(&len,4,1,file1);
                    char *filehash=malloc(len+1);
                    fread(filehash,1,len,file1);
                    *(filehash+len)='\0';
                    //读入文件名
                    fread(&len,4,1,file1);
                    char *FileName=malloc(len+1);
                    fread(FileName,1,len,file1);
                    free(destinationPath);
                    free(filehash);
                    free(FileName);
                }

                char *s=malloc(50);
                sprintf(s,"commit %s",head);

                //读入两个父提交哈希值
                memset(head,0,41);
                fread(head,1,40,file1);
                char *other=malloc(41);     
                fread(other,1,40,file1);                           
                //开始打印
                //打印commit
                printf("%s\n",s);
                //如果第二个不为零，就要打印merge
                if(strlen(other)!=0){
                    char *merge=malloc(30);
                    char *sheadhash=malloc(8);
                    char *stargethash=malloc(8);
                    strncpy(sheadhash,head,7);
                    strncpy(stargethash,other,7);
                    *(sheadhash+7)='\0';
                    *(stargethash+7)='\0';
                    sprintf(merge,"Merge: %s %s",sheadhash,stargethash);     
                    printf("%s\n",merge);               
                }
                //打印时间
                printf("%s\n",D);
                //打印消息
                printf("%s\n",message);
                //换行
                printf("\n");
        }
}

//checkout
int checkout(char *hash){
    //检查工作区是否“脏”
    //如果有脏改动
    if(dirty()){
        ERROR("The working directory is dirty");
        return -1;
    }
    else {
        ERROR("The working directory is clean");
    }
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
                    *(destinationPath+len)='\0';
                    //读入HASH
                    fread(&len,4,1,file1);
                    char *filehash=malloc(len+1);
                    fread(filehash,1,len,file1);
                    *(filehash+len)='\0';
                    //读入文件名
                    fread(&len,4,1,file1);
                    char *FileName=malloc(len+1);
                    fread(FileName,1,len,file1);
                    //利用哈希值把文件复制到新路径
                    char *SourcePath=malloc(520);
                    sprintf(SourcePath,"./.gitm/objects/file/%s",filehash);
                    if(copy_file(SourcePath,destinationPath)){
                        ERROR("fail to copy file\n");
                    };
                    free(destinationPath);
                    free(filehash);
                    free(FileName);
                    free(SourcePath);
                }
                fclose(file1);
                free(message);
                free(Date);

    //更新HEAD指针
    FILE *file2=fopen("./.gitm/refs/head/head","wb");
    fwrite(hash,1,40,file2);
    fclose(file2);
    return 0;

}


//compare
int compare(const void *a,const void *b){
    char **x=(char **)a;
    char **y=(char **)b;
    return(strcmp(*x,*y));
}


//merge
int merge(char *targethash){
    //获取头指针
    char *headhash=malloc(41);
    *(headhash+40)='\0';
    FILE* HEAD=fopen("./.gitm/refs/head/head","rb");
    fread(headhash,1,40,HEAD);
    fclose(HEAD);
    //HEAD 就是目标，无事发生，合并结束
    if(!strcmp(targethash,headhash)){
        ERROR("the target is head.");
        return 0;
    }
    //如果不是，利用递归找到target的所有祖先
    char **ancestorOfTarget=malloc(520*8);
    FindAncestor(targethash,ancestorOfTarget);
    //将自己写入祖先提交数组
    *(ancestorOfTarget+count)=malloc(41);
    *(*(ancestorOfTarget+count)+40)='\0';
    strcpy(*(ancestorOfTarget+count),targethash);
    count++;
    //从head提交开始往前找，每次都遍历
    //利用递归回溯head提交
    char **commonancestor=malloc(52*8);
    tracing(headhash,ancestorOfTarget,commonancestor);
    //现在获得了commonancestor,需要一步步回溯target，记录步数
    int array[52]={0};
    for(int k=0;k<countofancestor;k++){
        array[k]=steps(*(commonancestor+k),targethash);
    }
    //寻找最小步数的公共祖先
    int min=0;
    for(int k=1;k<countofancestor;k++){
        min=array[min]<array[k]?min:k;
    }
    //*(commonancestor+min)即为最近公共祖先的哈希值
    char *LCA=malloc(41);
    *(LCA+40)='\0';
    strcpy(LCA,*(commonancestor+min));
    //如果公共祖先是target,即HEAD 比目标新，则无事发生，合并结束
    if(!strcmp(*(commonancestor+min),targethash)){
        ERROR("head is outcome,nothing happened");
        return 0;
    }
    //如果公共祖先是head提交,则切换到target提交
    else if(!strcmp(*(commonancestor+min),headhash)){
        checkout(targethash);
        return 0;
    }
    //都不是的话执行下一步

    else {
        //首先检测冲突
        //记录LCA、target、head的文件名和哈希值
        //LCA:
        Getfile** fileofLCA=malloc(52*8);
        int numofLCA=getfile(fileofLCA,LCA);
        //target
        Getfile** fileoftarget=malloc(52);
        int numoftarget=getfile(fileoftarget,targethash);
        //head
        Getfile** fileofhead=malloc(52);
        int numofhead=getfile(fileofhead,headhash);

        //检测LCA和target的不同，同时记录名字和哈希值
        char **DiffBetLT=malloc(52*8);
        char **DiffLThash=malloc(52*8);
        int numofdifft=0;
        int numofLThash=0;
            //遍历target，在LCA中寻找(找到target中新加的和修改的)
            for(int i=0;i<numoftarget;i++){
                int flags=0;
                //在LCA中遍历寻找
                for(int j=0;j<numofLCA;j++){
                    //如果找到，则比较其哈希值
                    if(strcmp((*(fileoftarget+i))->name,(*(fileofLCA+j))->name)==0){
                        if(strcmp((*(fileoftarget+i))->hash,(*(fileofLCA+j))->hash)==0){
                            flags=1;
                            break;
                        }
                        else {
                            break;
                        }
                    }
                }
                if(flags==0){
                    *(DiffBetLT+numofdifft)=malloc(52);
                    *(DiffLThash+numofdifft)=malloc(41);
                    *(*(DiffLThash+numofdifft)+40)='\0';
                    strcpy(*(DiffBetLT+numofdifft),(*(fileoftarget+i))->name);
                    strcpy(*(DiffLThash+numofdifft),(*(fileoftarget+i))->hash);
                    numofdifft++;
                    numofLThash++;
                }
            }
            //遍历LCA，在target中寻找（找到target中删除的）
            char **deleteLT=malloc(20*8);
            int numofdeleteLT=0;
            for(int i=0;i<numofLCA;i++){
                int flags=0;
                for(int j=0;j<numoftarget;j++){
                    //如果找到了，说明存在，则不计入
                    if(strcmp((*(fileoftarget+j))->name,(*(fileofLCA+i))->name)==0){
                        flags=1;
                        break;
                    }
                }
                //如果没找到。则说明被删除了
                if(flags==0){
                    *(DiffBetLT+numofdifft)=malloc(52);
                    *(deleteLT+numofdeleteLT)=malloc(41);
                    *(*(deleteLT+numofdeleteLT)+40)='\0';
                    strcpy(*(DiffBetLT+numofdifft),(*(fileofLCA+i))->name);
                    strcpy(*(deleteLT+numofdeleteLT),(*(fileofLCA+i))->hash);
                    numofdifft++;
                    numofdeleteLT++;
                }
            }

        //检测LCA和head的不同
        char **DiffBetLH=malloc(52*8);
        char **DiffLHhash=malloc(52*8);
        int numofLHhash=0;
        int numofdiffh=0;
            //遍历head，在LCA中寻找(找到head中新加的和修改的)
            for(int i=0;i<numofhead;i++){
                int flags=0;
                //在LCA中遍历寻找
                for(int j=0;j<numofLCA;j++){
                    //如果找到，则比较其哈希值
                    if(strcmp((*(fileofhead+i))->name,(*(fileofLCA+j))->name)==0){
                        if(strcmp((*(fileofhead+i))->hash,(*(fileofLCA+j))->hash)==0){
                            flags=1;
                            break;
                        }
                        else {
                            break;
                        }
                    }
                }
                if(flags==0){
                    *(DiffBetLH+numofdiffh)=malloc(52);
                    *(DiffLHhash+numofLHhash)=malloc(41);
                    strcpy(*(DiffBetLH+numofdiffh),(*(fileofhead+i))->name);
                    strcpy(*(DiffLHhash+numofLHhash),(*(fileofhead+i))->hash);
                    numofLHhash++;
                    numofdiffh++;
                }
            }
            //遍历LCA，在head中寻找（找到head中删除的）
            char **deleteLH=malloc(20*8);
            int numofdeleteLH=0;
            for(int i=0;i<numofLCA;i++){
                int flags=0;
                for(int j=0;j<numofhead;j++){
                    //如果找到了，说明存在，则不计入
                    if(strcmp((*(fileofLCA+i))->name,(*(fileofhead+j))->name)==0){
                        flags=1;
                        break;
                    }
                }
                //如果没找到。则说明被删除了
                if(flags==0){
                    *(DiffBetLH+numofdiffh)=malloc(52);
                    *(deleteLH+numofdeleteLH)=malloc(41);
                    strcpy(*(deleteLH+numofdeleteLH),((*(fileofLCA+i))->hash));
                    strcpy(*(DiffBetLH+numofdiffh),(*(fileofLCA+i))->name);
                    numofdeleteLH++;
                    numofdiffh++;
                }
            }

        //比较这不同里是否有同一文件的修改
        //遍历DiffofBetLT,在DiffofBetLH中寻找
        int flag=0;
        for(int i=0;i<numofdifft;i++){
            for(int j=0;j<numofdiffh;j++){
                //如果找到，说明有冲突
                if(strcmp(*(DiffBetLT+i),*(DiffBetLH+j))==0){
                    flag=1;
                    break;
                }
            }
            if (flag == 1) {
                break;
            }
        }

        //如果有冲突，则输出ERROR
        if(flag==1){
            ERROR("commits conflicts\n");
            return -1;
        }
        //如果没有冲突，则就开始考虑合并
        else {
            //用回调函数删除当前工作区中内容
            walk_path(".",path_delete, NULL);
            //开始反序列化target,只反序列化存在于DiffLThash中的内容

                //先对DiffLThash进行排序，方便查找
                qsort(DiffLThash,numofLThash,8,compare);

            //以二进制只读模式打开target版本的commit对象
            char *findpath=malloc(520);
            sprintf(findpath,"./.gitm/objects/commit/%s",targethash);
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
                    *(destinationPath+len)='\0';
                    //读入HASH
                    fread(&len,4,1,file1);
                    char *filehash=malloc(len+1);
                    fread(filehash,1,len,file1);
                    *(filehash+len)='\0';
                    //读入文件名
                    fread(&len,4,1,file1);
                    char *FileName=malloc(len+1);
                    fread(FileName,1,len,file1); 
                    //判断是否在DiffLThash中
                    if(bsearch(&filehash,DiffLThash,numofLThash,8,compare)!=NULL){
                        //如果是，便反序列化到工作区
                    //利用哈希值把文件复制到新路径
                    char *SourcePath=malloc(520);
                    sprintf(SourcePath,"./.gitm/objects/file/%s",filehash);
                    if(copy_file(SourcePath,destinationPath)){
                        ERROR("fail to copy file\n");
                    };
                    free(destinationPath);
                    free(filehash);
                    free(FileName);
                    free(SourcePath);                        
                    }                   
                }
                fclose(file1);
                free(message);
                free(Date);                 

            //开始反序列化head,只反序列化存在于DiffLHhash中的内容
                //先对DiffLHhash进行排序，方便查找
                qsort(DiffLHhash,numofLHhash,8,compare);

                //以二进制只读模式打开head版本的commit对象
            findpath=malloc(520);
            sprintf(findpath,"./.gitm/objects/commit/%s",headhash);
            file1=fopen(findpath,"rb");
            //开始反序列化
                //读入提交消息
                fread(&len,4,1,file1);
                message=malloc(len+1);
                fread(message,1,len,file1);
                //读入时间戳
                fread(&len,4,1,file1);
                Date=malloc(len+1);
                fread(Date,1,len,file1);
                //读入文件个数
                fread(&NumOfFile,4,1,file1);
                //处理NumOfFile个文件
                for(int i=0;i<NumOfFile;i++){
                    //读入原路径
                    fread(&len,4,1,file1);
                    char *destinationPath=malloc(len+1);
                    fread(destinationPath,1,len,file1);
                    *(destinationPath+len)='\0';
                    //读入HASH
                    fread(&len,4,1,file1);
                    char *filehash=malloc(len+1);
                    fread(filehash,1,len,file1);
                    *(filehash+len)='\0';
                    //读入文件名
                    fread(&len,4,1,file1);
                    char *FileName=malloc(len+1);
                    fread(FileName,1,len,file1);
                    //判断是否在DiffLHhash中
                    if(bsearch(&filehash,DiffLHhash,numofLHhash,8,compare)!=NULL){
                        //如果是，便反序列化到工作区
                    //利用哈希值把文件复制到新路径 
                    char *SourcePath=malloc(520);
                    sprintf(SourcePath,"./.gitm/objects/file/%s",filehash);
                    if(copy_file(SourcePath,destinationPath)){
                        ERROR("fail to copy file\n");
                    };
                    free(destinationPath);
                    free(filehash);
                    free(FileName);
                    free(SourcePath); 
                    }                    
                }
                fclose(file1);
                free(message);
                free(Date);

            //开始反序列化最近公共祖先（*(commonancestor+min)）
                //先对DiffBetLT和DiffBetLH排序，便于查找
                qsort(DiffBetLT,numofdifft,8,compare);
                qsort(DiffBetLH,numofdiffh,8,compare);


                //以二进制只读模式打开最近公共祖先版本的commit对象
            findpath=malloc(520);
            sprintf(findpath,"./.gitm/objects/commit/%s",*(commonancestor+min));
            file1=fopen(findpath,"rb");     
            //开始反序列化
                //读入提交消息
                fread(&len,4,1,file1);
                message=malloc(len+1);
                fread(message,1,len,file1);
                //读入时间戳
                fread(&len,4,1,file1);
                Date=malloc(len+1);
                fread(Date,1,len,file1);
                //读入文件个数
                fread(&NumOfFile,4,1,file1);
                //处理NumOfFile个文件
                for(int i=0;i<NumOfFile;i++){
                    //读入原路径
                    fread(&len,4,1,file1);
                    char *destinationPath=malloc(len+1);
                    fread(destinationPath,1,len,file1);
                    *(destinationPath+len)='\0';
                    //读入HASH
                    fread(&len,4,1,file1);
                    char *filehash=malloc(len+1);
                    fread(filehash,1,len,file1);
                    *(filehash+len)='\0';
                    //读入文件名
                    fread(&len,4,1,file1);
                    char *FileName=malloc(len+1);
                    fread(FileName,1,len,file1);
                    *(FileName+len)='\0';
                    //当文件名不在DiffBetLT和DiffBetLH时，才会反序列化
                    if((bsearch(&FileName,DiffBetLT,numofdifft,8,compare)==NULL)&&(bsearch(&FileName,DiffBetLH,numofdiffh,8,compare)==NULL)){
                        char *SourcePath=malloc(520);
                        sprintf(SourcePath,"./.gitm/objects/file/%s",filehash);
                        if(copy_file(SourcePath,destinationPath)){
                            ERROR("fail to copy file\n");
                        }
                        free(SourcePath);
                    }
                    free(destinationPath);
                    free(filehash);
                    free(FileName);
                }
                fclose(file1);
                free(message);
                free(Date);                
        }
    }
    
    //开始反序列化本次提交  
    //定义一个commit结构体并记录元数据
    CommitStruct STR;
    STR.message=malloc(52);
    memset(STR.message,0,52);
    STR.Time=malloc(52);
    //消息
    sprintf(STR.message,"Merged two commits.");
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

    //计算文件大小
    int n=0;
    //在./.gitm/temp下创建一个文件tempcommit
    FILE *file1=fopen("./.gitm/temp/tempcommit","wb");
    //写入提交消息
    int len=strlen(STR.message);
    fwrite(&len,4,1,file1);
    fwrite(STR.message,1,len,file1);
    n+=(len+1);
    //写入时间戳
    len=strlen(STR.Time);
    fwrite(&len,4,1,file1);
    fwrite(STR.Time,1,len,file1);
    n+=(len+1);
    //写入文件个数
    fwrite(&NumOfFile,sizeof(int),1,file1);
    n+=sizeof(int);
    //写入链表中NumOfFile个文件对象结构体
    FileStruct *ptr=STR.ptr;
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

        //写入父提交的两个哈希值((head在前))
        //追加模式
        FILE *file2=fopen(newpath,"a");
        //写入：
        fwrite(headhash,1,40,file2);//写入head父提交
        fwrite(targethash,1,40,file2);//写入target父提交
        fclose(file2);

        //更新头指针
        file1=fopen("./.gitm/refs/head/head","wb");
        fwrite(CommitSum,1,40,file1);
        fclose(file1);

        //录入log
        file1=fopen("./.gitm/logs/logs","a");
        //c:commit;flagM:Merge;D:date;in:message
            //commit 
        char *s=malloc(50);
        sprintf(s,"commit %s",CommitSum);
        fwrite(s,1,47,file1);
        free(s);   
        //merge:如果flag为1，说明有
        int flagM=1;
        fwrite(&flagM,4,1,file1);
        char *merge=malloc(30);
        char *sheadhash=malloc(8);
        char *stargethash=malloc(8);
        strncpy(sheadhash,headhash,7);
        strncpy(stargethash,targethash,7);
        *(sheadhash+7)='\0';
        *(stargethash+7)='\0';
        sprintf(merge,"Merge: %s %s",sheadhash,stargethash);
        fwrite(merge,1,22,file1);
        free(merge);
        //Date:
        len=strlen(STR.Time)+6;
        fwrite(&len,4,1,file1);
        char *D=malloc(len+1);
        sprintf(D,"Date: %s",STR.Time);
        fwrite(D,1,len,file1);
        free(D);
        //message:
        len=strlen(STR.message);
        fwrite(&len,4,1,file1);
        fwrite(STR.message,1,len,file1);
        fclose(file1);
       //为logs计数
        file1=fopen("./.gitm/logs/count","a+");
        int temp;
        fread(&temp,4,1,file1);
        temp++;
        fclose(file1);
        file1=fopen("./.gitm/logs/count","w");
        fwrite(&temp,4,1,file1);
        fclose(file1);
        return 0;
}