#include <iostream>
#include <string>
#include "../SharedMemory.h"
#include "../List.h"


int main(int argc, char const * argv[])
{
  List<int> list = List<int>::createListInShm("../database.db");
  list.add(10);
  list.add(20);
  list.add(30);
  list.add(40);
  list.print();

  shmBlock::freeShm(4096 * 2);

  return 0;
}
