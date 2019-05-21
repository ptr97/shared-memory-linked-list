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
  static long long offset;

  static void allocateMemory() 
  {
    const int fd = open("database.db", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if( fd < 0 ) {
      std::cout << "Open DB error" << std::endl;
      exit(-1);
    }

    const int status = ftruncate(fd, static_cast<off_t>(10240));
    if(status != 0) {
      std::cout << "ftrucate error" << std::endl;
      exit(-1);
    }

    shmBlock::startPtr = (char *) mmap(0, 10240, PROT_WRITE, MAP_SHARED, fd, 0);
    if(shmBlock::startPtr == MAP_FAILED)
    {
      std::cout << "mmap failed" << std::endl;
      exit(-1);
	  }

    memcpy(shmBlock::startPtr, &shmBlock::startPtr, sizeof(&shmBlock::startPtr));
    shmBlock::lastUsed = shmBlock::startPtr + sizeof(&shmBlock::startPtr);
    std::cout << "shmBlock::startPtr => " << (void *) shmBlock::startPtr << std::endl;
    std::cout << "shmBlock::lastUsed => " << (void *) shmBlock::lastUsed << std::endl;
  }

  static void readFromMemory()
  {
    const int fd = open("database.db", O_RDONLY, S_IRUSR | S_IWUSR);
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

    shmBlock::setShmStartPtr();

    for(char * p = shmBlock::startPtr; p < shmBlock::startPtr + 20; p +=4) {
      std::cout << std::hex << (u_int32_t) *p;
    }
    std::cout << std::endl;
  }


  static void setShmStartPtr()
  {
    char * old = nullptr;
    memcpy(&old, shmBlock::startPtr, sizeof(char *));

    std::cout<< "old => " << (void *)  old << std::endl;
    std::cout << "new => " << (void *) shmBlock::startPtr << std::endl;
    
    shmBlock::offset = shmBlock::startPtr - old - sizeof(int *) + 8;

    std::cout << "offset = " << offset << std::endl;

    shmBlock::lastUsed = shmBlock::startPtr + sizeof(shmBlock::startPtr);

    std::cout << "new start pointer = " << (void *) shmBlock::startPtr << std::endl;
    std::cout << "new last used pointer = " << (void *) shmBlock::lastUsed << std::endl;
  }

  static void freeShm()
  {
    if(munmap(shmBlock::startPtr, static_cast<off_t>(10240))) 
    {
      std::cout << "unmap error" << std::endl;
      exit(-1);
	  }
  }
};

#endif
