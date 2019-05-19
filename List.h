#ifndef __LIST_H__
#define __LIST_H__

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>


template <typename T>
class List
{
  template <typename U>
  struct Node 
  {
    Node(const U u, int offset)
    {
      this->value = u;
      this->selfOffset = offset;
      this->nextOffset = offset + 1;
      memcpy(shmBlock::startPtr + selfOffset * sizeof(this), this, sizeof(this));
    }

    bool isLast()
    {
      return this->nextOffset == -1;
    }

    Node<U> * getSelfAddress()
    {
      return (Node<U> *) shmBlock::startPtr + selfOffset * sizeof(this);
    }

    Node<U> * getNextAdress()
    {
      if(! this->isLast()) {
        return (Node<U> *) shmBlock::startPtr + nextOffset * sizeof(this); 
      } else {
        return nullptr;
      }
    }

    U value;
    int nextOffset = -1;
    int selfOffset = -1;
  };

  struct Meta
  {

  };

public:
  // ~List() 
  // {
  //   Node<T> * iter = head;
  //   Node<T> * temp = nullptr;

  //   while(iter != nullptr) {
  //     temp = iter;
  //     iter = iter->next;
  //     delete temp;
  //   }
  // }

  void add(T item) 
  {
    Node<T> newNode(item, m_size++);

    // if(m_size == 0) {
    //   head = newNode.getSelfAddress();
    // } else {
    //   Node<T> * iter = head->getNextAddress();
    //   while(iter != nullptr) {
    //     std::cout << iter->value << std::endl;
    //     iter = iter->getNextAddress();
    //   }
    //   iter->next = newNode;
    // }
  }

  void print() 
  {
    Node<T> * iter = (Node<T> *) shmBlock::startPtr;
    int counter = 0;
    while(iter != nullptr) {
      std::cout << counter++ << ": " << iter->value << std::endl;
      iter = iter->getNextAdress();
    }
  }

private:
  Node<T> * head = nullptr;
  unsigned int m_size = 0;
};



#endif
