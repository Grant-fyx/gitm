#include "gitm.h"

static char const GITM_DIR[] = ".gitm"; // make sure this is the name of your gitm directory

//判断指定路径所对应的文件或目录是否存在
bool exists(char const *path) {
  struct stat s;
  return stat(path, &s) == 0;
  //stat函数表示获取指定路径对应的文件或目录的状态信息（path即为指向该路径的指针），并填充到传入的 struct stat 结构体中。成功返回 0，失败返回 - 1
}//存在返回true，不存在返回false

//实现copy_file函数将一个文件从源路径（src_path）复制到目标路径（dest_path）
int copy_file(char const *src_path, char const *dest_path) {
  //这里使用fopen函数以二进制只读模式
  FILE *src = fopen(src_path, "rb");
  //如果打开失败则输出错误信息
  if (src == NULL) {
    ERROR("failed to open file %s\n", src_path);
    return -1;
  }

  char path[PATH_MAX];
  for (char const *slash = strchr(dest_path, '/'); *slash != '\0'; slash = strchr(slash + 1, '/')) {
    strncpy(path, dest_path, slash - dest_path);
    path[slash - dest_path] = '\0';
    if (!exists(path) && make_directory(path) != 0) {
      ERROR("failed to create directory %s\n", path);
      fclose(src);
      return -1;
    }
  }
  FILE *dest = fopen(dest_path, "wb");
  if (dest == NULL) {
    ERROR("failed to open file %s\n", dest_path);
    fclose(src);
    return -1;
  }
  char buf[BUFSIZ];
  size_t n;
  while ((n = fread(buf, 1, sizeof(buf), src)) > 0) {
    if (fwrite(buf, 1, n, dest) != n) {
      ERROR("failed to write to file %s\n", dest_path);
      fclose(src);
      fclose(dest);
      return -1;
    }
  }
  fclose(src);
  fclose(dest);
  return 0;
}


//文件从原来的目录移动到新的目录下，并同时更改文件名。（也可以不移动只修改文件名或者只移动不修改文件名）
int rename_file(char const *old_path, char const *new_path) {
  return rename(old_path, new_path);
  //如果重命名操作成功，rename函数返回 0。
}

//删除指定路径下的文件
int remove_file(char const *path) {
  return remove(path);
  //如果文件删除成功，remove函数返回 0
}

//给定路径下创建一个文件夹（目录），成功创建目录时返回0
int make_directory(char const *path) {
#if defined(__unix__)
  return mkdir(path, 0755);
#elif defined(_WIN32) || defined(_WIN64)
  return mkdir(path);
#endif
}

//删除指定路径下的目录（必须为空）
int remove_directory(char const *path) {
  return rmdir(path);
}


//回调函数
int walk_path(char const *path, callback_t callback, void *arg) {
  DIR *dir = opendir(path);
  if (dir == NULL) {
    ERROR("failed to open directory %s\n", path);
    return -1;
  }
  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    struct stat s;
    char new_path[PATH_MAX];
    snprintf(new_path, PATH_MAX, "%s/%s", path, entry->d_name);
    if (stat(new_path, &s) != 0) {
      ERROR("failed to get the status of file %s\n", entry->d_name);
      return -1;
    }
    if (s.st_mode & S_IFDIR) {
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, GITM_DIR) == 0) {
        continue;
      }
      walk_path(new_path, callback, arg);
    } else if (s.st_mode & S_IFREG) {
      callback(new_path, arg);
    } else {
      ERROR("unknown file type %s\n", entry->d_name);
      return -1;
    }
  }
  closedir(dir);
  return 0;
}

//计算给定数据的 SHA-1 哈希值，并将结果以十六进制字符串的形式存储到指定的字符数组sum中
int sha1sum(char *sum, void const *data, size_t size) {
  SHA_CTX const *ctx = (SHA_CTX *)SHA1(data, size, NULL);
  if (ctx == NULL) {
    ERROR("failed to calculate sha1sum\n");
    return -1;
  }
  unsigned char const *buf = (unsigned char const *)ctx;
  for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
    sprintf(sum + i * 2, "%02x", buf[i]);
  }
  return 0;
}

//获取当前的时间信息，并将其按照指定的格式转换为字符串后存储到传入的字符数组 buf 中
int timestamp(char *buf) {
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  strftime(buf, TIMESTAMP_LEN, "%a %b %d %H:%M:%S %Y", tm);
  return 0;
}
