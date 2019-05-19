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
  static char * lastUsed;

  static void allocateMemory(const char * key, uint size) 
  {
    const int fd = open(key, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if( fd < 0 ) {
      std::cout << "Open DB error" << std::endl;
      exit(-1);
    }

    const int status = ftruncate(fd, static_cast<off_t>(size));
    if(status != 0) {
      std::cout << "ftrucate error" << std::endl;
      exit(-1);
    }

    shmBlock::startPtr = (char *) mmap(0, size, PROT_WRITE, MAP_SHARED, fd, 0);
    if(shmBlock::startPtr == MAP_FAILED)
    {
      std::cout << "mmap failed" << std::endl;
      exit(-1);
	  }
  }

  static void readFromMemory(const char * key)
  {
    const int fd = open(key, O_RDONLY, S_IRUSR | S_IWUSR);
    if( fd < 0 ) {
      std::cout << "Open DB error" << std::endl;
      exit(-1);
    }

    struct stat file_statistics;
    fstat(fd, &file_statistics);

    shmBlock::startPtr = (char *) mmap(0, file_statistics.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if(shmBlock::startPtr == MAP_FAILED)
    {
      std::cout << "mmap failed" << std::endl;
      exit(-1);
	  }
  }

  static void freeShm(uint size)
  {
    if(munmap(shmBlock::startPtr, static_cast<off_t>(size))) 
    {
      std::cout << "unmap error" << std::endl;
      exit(-1);
	  }
  }
};

#endif
