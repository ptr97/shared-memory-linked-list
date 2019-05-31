#include <iostream>
#include "../SharedMemory.h"
#include "../List.h"


int main()
{
  const uint shmBlockSize = 4096 * 2;
  
  List<int> listFromMem = List<int>::readListFromMemory("../database.db");
  listFromMem.print();
  shmBlock::freeShm(shmBlockSize);

  return 0;
}
