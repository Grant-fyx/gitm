#include "gitm.h"
#include "repository.h"
int main(int argc, char *argv[]) {
  //错误的参数
  if (argc < 2) {
    ERROR("Usage: gitm <command>\n");
    return 1;
  }

  //version
  if (!strcmp(argv[1], "version")) {
    printf("%s",
#include "../icon.txt"
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

  return 0;
}
