#ifndef COMMIT_H
#define COMMIT_H

#include "blob.h"
typedef struct B{
    char *Time;
    char *message;
    FileStruct *ptr;
}CommitStruct;

#endif