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
    char *message=malloc(strlen(argv[2])+1);
    strcpy(message,argv[2]);
    commit(message);
  }
  return 0;
}
