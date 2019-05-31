#include <iostream>
#include <string>
#include "SharedMemory.h"
#include "List.h"


int main()
{
  const uint shmBlockSize = 4096 * 2;

  List<int> list = List<int>::createListInShm("database.db", shmBlockSize);
  list.add(10);
  list.add(20);
  list.add(30);
  list.add(40);
  list.print();
  std::cout << "list.exists(20) == " << list.exists(20) << std::endl;
  list.remove(20);
  list.remove(10);
  list.print();
  shmBlock::freeShm(shmBlockSize);

  std::cout << std::endl << "Now we will read linked list from memory" << std::endl;
  List<int> listFromMem = List<int>::readListFromMemory("database.db");
  listFromMem.print();
  shmBlock::freeShm(shmBlockSize);

  return 0;
}
