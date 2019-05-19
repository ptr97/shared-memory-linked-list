#include <iostream>
#include <string>
#include "../SharedMemory.h"
#include "../List.h"


int main(int argc, char const * argv[])
{
  List<int> listFromMem = List<int>::readListFromMemory("../database.db");
  listFromMem.print();
  shmBlock::freeShm(4096 * 2);

  return 0;
}
