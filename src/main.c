#include "gitm.h"
#include "repository.h"
#include "blob.h"
int main(int argc, char *argv[]) {
  //错误的参数
  if (argc < 2) {
    ERROR("Usage: gitm <command>\n");
    return 1;
  }

  //version
  if (!strcmp(argv[1], "version")) {
    printf("%s",
#include "../like.txt"
    );
  }

  //init
  else if(!strcmp(argv[1],"init")){
    //返回非零则说明创建失败
    if(init()){
      ERROR("Creation failed or already exist .gitm");
    }
  }

  //commit
  else if(!strcmp(argv[1],"commit")){
    int len=0;
    for(int i=2;i<=argc-1;i++){
      len+=strlen(argv[i]);
    }
    char *message=malloc(len+argc);
    memset(message,0,len+argc);
    for(int i=2;i<=argc-1;i++){
      strcat(message,argv[i]);
      strcat(message," ");
    }
    commit(message);
  }

  //log
  else if(!strcmp(argv[1],"log")){
    logs();
  }

  //checkout
  else if(!strcmp(argv[1],"checkout")){
    checkout(argv[2]);
  }

  //merge
  else if(!strcmp(argv[1],"merge")){
    //先看工作区是否干净
    if(dirty()){
        ERROR("The working directory is dirty");
        return 0;
    }
    merge(argv[2]);
  }
  return 0;
}
