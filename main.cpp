#include <iostream>
#include <string>
#include "SharedMemory.h"
#include "List.h"


int main(int argc, char const * argv[])
{
  shmBlock::allocateMemory("database.db", 4096 * 2);

  List<int> list;
  list.add(10);
  list.add(20);
  list.add(30);
  list.add(40);
  list.print();

  shmBlock::freeShm(4096 * 2);

  return 0;
}
