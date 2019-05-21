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
  static int offset;

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
    
    shmBlock::offset = shmBlock::startPtr - old - sizeof(int *);

    std::cout << "offset = " << offset << std::endl;

    shmBlock::lastUsed = shmBlock::startPtr + sizeof(shmBlock::startPtr);

    std::cout << (void *) shmBlock::startPtr << std::endl;
    std::cout << (void *) shmBlock::lastUsed << std::endl;
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

template<typename T>
class shmPtr {

public:
  T * m_data = nullptr;

  shmPtr<T>()
  {
    m_data = nullptr;
  }

  shmPtr<T>(std::nullptr_t)
  {
    m_data = nullptr;
  }

  shmPtr<T>(T * ptr)
  {
    makeNew();
    memcpy(m_data, ptr, sizeof(T));
  }

  void operator=(std::nullptr_t)
  {
    m_data = nullptr;
  }

  const T operator*() const
  {
    // return *m_data;

    return *(m_data + shmBlock::offset);
  }

  T operator*()
  { 
    // return *m_data;

    return *(m_data + shmBlock::offset);
  }
  T* operator->()
  {
    // return m_data;

    std::cout << m_data + shmBlock::offset << std::endl;

    return (m_data + shmBlock::offset);
  }

  bool operator==(std::nullptr_t) const
  {
    return (m_data + shmBlock::offset) == nullptr;
  }

  bool operator!=(std::nullptr_t) const
  {
    return (m_data + shmBlock::offset) != nullptr;
  }

private:
  void makeNew()
  {
    // m_data = (T*) malloc(sizeof(T));


    m_data = (T*) shmBlock::lastUsed;
    std::cout << "m_data = " << m_data << std::endl;
    shmBlock::lastUsed += sizeof(T);
  }
  
};

#endif
