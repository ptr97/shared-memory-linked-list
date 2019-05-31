#include "SharedMemory.h"

char * shmBlock::startPtr;

void shmBlock::allocateMemory(const char * key, uint size) {
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
  if(shmBlock::startPtr == MAP_FAILED) {
    std::cout << "mmap failed" << std::endl;
    exit(-1);
  }

  memset(shmBlock::startPtr, '\0', static_cast<off_t>(size));
  std::cout << "Memory block of size " << size << " allocated at: " << (void *) shmBlock::startPtr << std::endl;
}

uint shmBlock::readFromMemory(const char * key) {
  const int fd = open(key, O_RDONLY, S_IRUSR | S_IWUSR);
  if(fd < 0) {
    std::cout << "Open DB error" << std::endl;
    exit(-1);
  }

  struct stat file_statistics;
  fstat(fd, &file_statistics);

  shmBlock::startPtr = (char *) mmap(0, file_statistics.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if(shmBlock::startPtr == MAP_FAILED) {
    std::cout << "mmap failed" << std::endl;
    exit(-1);
  }
  std::cout << "Memory block of size " << file_statistics.st_size << " read at: " << (void *) shmBlock::startPtr << std::endl;
  return file_statistics.st_size;
}

void shmBlock::freeShm(uint size) {
  if(munmap(shmBlock::startPtr, static_cast<off_t>(size))) {
    std::cout << "unmap error" << std::endl;
    exit(-1);
  }
  std::cout << "Memory block has been released..." << std::endl;
}
