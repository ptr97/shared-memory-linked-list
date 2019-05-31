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
class List {
  template <typename U>
  struct Node {
    Node(const U u, int offset) {
      this->value = u;
      this->nextOffset = offset;
    }

    U value;
    int nextOffset = -1;
    bool saved = true;
  };

  struct Meta {
    unsigned int head;
  };

public:
  static List<T> createListInShm(const char * shmName, uint shmBlockSize) {
    shmBlock::allocateMemory(shmName, shmBlockSize);
    return List(true);
  }

  static List<T> readListFromMemory(const char * shmName) {
    shmBlock::readFromMemory(shmName);
    return List(false);
  }

  void add(T item) {
    int freeOffset = findFreeOffset();
    
    std::cout << "freeOffset = " << freeOffset << std::endl;
    Meta & meta = getMeta();
    Node<T> & newNode = getNode(freeOffset);
    newNode.value = item;
    newNode.nextOffset = meta.head;
    newNode.saved = true;

    meta.head = freeOffset;
  }

  bool exists(T item) {
    Meta & meta = getMeta();
    int current = meta.head;
    while (current != -1) {
      const Node<T> & currentNode = getNode(current);
      if (currentNode.value == item) {
        return true;
      } else {
        current = currentNode.nextOffset;
      }
    }
    return false;
  }

  bool remove(T item) {
    Meta & meta = getMeta();
    int current = meta.head;
    int last = meta.head;
    while (current != -1) {
      Node<T> & currentNode = getNode(current);
      if (currentNode.value == item) {
        std::cout << "removing node with value " << currentNode.value << std::endl;
        currentNode.saved = false;
        Node<T> & lastNode = getNode(last);
        lastNode.nextOffset = currentNode.nextOffset;
        return true;
      } else {
        last = current;
        current = currentNode.nextOffset;
      }
    }
    return false;
  }

  void print() {
    Meta & meta = getMeta();
    int current = meta.head;
    while (current != -1) {
      const Node<T> & currentNode = getNode(current);
      std::cout << "Current: " << currentNode.value << std::endl;
      current = currentNode.nextOffset;
    }
  }

private:
  List(bool newList) {
    struct Meta & meta = getMeta();

    if (newList) {
      meta.head = -1;

      for(int i = 0; i < maxSize; ++i) {
        getNode(i).saved = false;
      }
    }
  }

  Node<T> & getNode(int offset) {
    return *reinterpret_cast<Node<T>*>(shmBlock::startPtr + sizeof(Meta) + sizeof(Node<T>) * offset);
  }

  Meta & getMeta() {
    return *reinterpret_cast<Meta *>(shmBlock::startPtr);
  }

  int findFreeOffset() {
    int next_offset = 0;
    while (getNode(next_offset).saved != false) {
      if (next_offset > maxSize) {
        std::cout << "ERROR: Out of shm memory" << std::endl;
        exit(-1);
      }
      ++next_offset;
    }
    return next_offset;
  }

  int maxSize = 20;
};



#endif
