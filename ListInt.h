#ifndef __LIST_INT_H__
#define __LIST_INT_H__

#include "SharedMemory.h"


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
    shmPtr<Node> next = nullptr;
  };

public:
  ListInt() {}

  ListInt(shmPtr<Node> ptrToShm) 
  {
    m_head = ptrToShm;
  }

  ~ListInt() 
  {
    shmPtr<Node> iter = m_head;
    shmPtr<Node> temp = nullptr;

    while(iter != nullptr) {
      temp = iter;
      iter = iter->next;
      temp.remove();
    }
  }

  void add(int item) 
  {
    Node * n = new Node(item);
    shmPtr<Node> newNode(n);
    ++m_size;

    if(m_head == nullptr) {
      m_head = newNode;
    } else {
      shmPtr<Node> iter = m_head;
      while(iter->next != nullptr) {
        iter = iter->next;
      }
      iter->next = newNode;
    }
  }

  void print() 
  {
    shmPtr<Node> iter = m_head;
    int counter = 0;
    while(iter != nullptr) {
      std::cout << counter++ << ": " << iter->value << std::endl;
      iter = iter->next;
    }
  }

private:
  shmPtr<Node> m_head = nullptr;
  unsigned int m_size = 0;
};


#endif
