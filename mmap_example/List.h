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
  List(T item)
  {
    Node<T> firstNode(item, 0);
    head = firstNode.getSelfAddress();
    m_size++;
    std::cout << "Tworzenie listy m_size = " << m_size << " firstNode.getSelfAddress() = " << firstNode.getSelfAddress() << std::endl;
  }
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
    std::cout << "new node address = " << (void *) newNode.getSelfAddress() << std::endl;
    std::cout << "node = " << newNode.value << " , " << newNode.selfOffset << " , " << newNode.nextOffset << std::endl;
    std::cout << newNode.getSelfAddress() << std::endl;
    std::cout << newNode.getNextAdress() << std::endl;

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
    Node<T> * iter = head;
    std::cout << "inside print head adress = " << (void *) head << std::endl;
    int counter = 0;
    while(iter != nullptr && counter < 10) {
      std::cout << counter++ << ": " << iter->value << " , " << iter->selfOffset << " , " << iter->nextOffset << std::endl;
      iter = iter->getNextAdress();
      std::cout << "iter = " << (void *) iter << std::endl;
    }
  }

private:
  Node<T> * head = nullptr;
  unsigned int m_size = 0;
};



#endif
