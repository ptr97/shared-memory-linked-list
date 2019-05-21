#include <iostream>
#include <string>
#include "SharedMemory.h"
#include "ListInt.h"


int main(int argc, char const * argv[])
{
  // shmBlock::allocateMemory();

  // std::cout << "Shared memory linked list - UNIX 2nd project" << std::endl;
  // ListInt list = ListInt();

  // list.add(10);
  // list.add(20);
  // list.add(30);
  // list.add(40);
  // list.print();

  // shmBlock::freeShm();

  shmBlock::readFromMemory();

  shmPtr<ListInt::Node> * test = reinterpret_cast<shmPtr<ListInt::Node> *>(shmBlock::lastUsed);
  std::cout << test->m_data << std::endl;
  std::cout << reinterpret_cast<shmPtr<ListInt::Node> *>(shmBlock::lastUsed) << std::endl;
  ListInt listFromMem = ListInt(*reinterpret_cast<shmPtr<ListInt::Node> *>(shmBlock::lastUsed));
  listFromMem.print();

  shmBlock::freeShm();

  return 0;
}
