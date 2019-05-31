#ifndef __SHARED_MEMORY_H__
#define __SHARED_MEMORY_H__

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <unistd.h>


class shmBlock 
{
public:
  static char * startPtr;

  static void allocateMemory(const char * key, uint size);
  static uint readFromMemory(const char * key);
  static void freeShm(uint size);
};

#endif
