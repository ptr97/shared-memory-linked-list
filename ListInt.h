#ifndef __LIST_INT_H__
#define __LIST_INT_H__

#include "SharedMemory.h"
#include "shmPtr.h"


class ListInt 
{
public:
  struct Node
  {
    Node(const int i)
    {
      this->value = i;
    }

    int value;
    int r = 0xa5a5a5a5;
    shmPtr<Node> next = shmPtr<Node>((Node *) nullptr);
  };

public:
  ListInt(int p) 
  {
    head = shmPtr<Node>((Node *) new (shmBlock::lastUsed) Node(p));
    shmBlock::lastUsed += sizeof(Node);
  }
  void add(int item) 
  {
    shmPtr<Node> newNode = new (shmBlock::lastUsed) Node(item);
    shmBlock::lastUsed += sizeof(Node);

    shmPtr<Node> iter = shmPtr<Node>((Node *) head);
    std::cout << "head value = " << (*iter).value << std::endl;
    std::cout << "(*iter).next = " << (*iter).next << " iter.value " << (*iter).value <<  std::endl;

    while((*iter).next != nullptr) {
      std::cout << "loooooop" << std::endl;
      iter = shmPtr<Node>((Node *) (*iter).next);
    }
    
    (*iter).next = newNode; // ???
    std::cout << "head value = " << (*iter).value << std::endl;
  }

  void print() 
  {
    shmPtr<Node> iter = shmPtr<Node>((Node *) head);
    int counter = 0;
    
    while(iter != nullptr) {
      std::cout << counter++ << ": " << (*iter).value << std::endl;
      iter = shmPtr<Node>((Node *) (*iter).next);
    }
  }

private:
  shmPtr<Node> head = shmPtr<Node>((Node *) nullptr);
};


#endif
