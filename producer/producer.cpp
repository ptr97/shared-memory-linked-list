#include <iostream>
#include "../SharedMemory.h"
#include "../List.h"


int main()
{
  const uint shmBlockSize = 4096 * 2;

  List<int> list = List<int>::createListInShm("../database.db", shmBlockSize);
  list.add(10);
  list.add(20);
  list.add(30);
  list.add(40);
  list.print();

  shmBlock::freeShm(shmBlockSize);

  return 0;
}
