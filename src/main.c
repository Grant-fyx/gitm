#include "gitm.h"

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
  
  return 0;
}
