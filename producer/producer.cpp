#include <iostream>
#include <string>
#include "../SharedMemory.h"
#include "../List.h"


int main(int argc, char const * argv[])
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
