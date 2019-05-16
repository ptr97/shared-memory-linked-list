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
    Node(const U u)
    {
      this->value = u;
    }

    U value;
    Node<U> * next = nullptr;
    Node<U> * self = this;
  };

public:
  ~List() 
  {
    Node<T> * iter = head;
    Node<T> * temp = nullptr;

    while(iter != nullptr) {
      temp = iter;
      iter = iter->next;
      delete temp;
    }
  }

  void add(T item) 
  {
    Node<T> * newNode = new Node<T>(item);
    ++m_size;

    if(head == nullptr) {
      head = newNode;
    } else {
      Node<T> * iter = head;
      while(iter->next != nullptr) {
        std::cout << iter->value << std::endl;
        iter = iter->next;
      }
      iter->next = newNode;
    }
  }

  void print() 
  {
    Node<T> * iter = head;
    int counter = 0;
    while(iter != nullptr) {
      std::cout << counter++ << ": " << iter->value << std::endl;
      iter = iter->next;
    }
  }

  void serialize(const char * dbName) 
  {
    const int fd = openDbInWriteMode(dbName);
    truncate(fd);
    dump(fd);
    close(fd);
  }

  void deserialize(const char * dbName)
  {
    const int fd = openDbInReadMode(dbName);
    Node<T> * shared = readFromDb(fd);
  }

private:
  Node<T> * head = nullptr;
  unsigned int m_size = 0;

  int openDbInWriteMode(const char * dbName) 
  {
    const int fd = open(dbName, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if( fd < 0 ) {
      std::cout << "Open DB error" << std::endl;
      exit(-1);
    }
    return fd;
  }

  int openDbInReadMode(const char * dbName)
  {
    const int fd = open(dbName, O_RDONLY, S_IRUSR | S_IWUSR);
    if( fd < 0 ) {
      std::cout << "Open DB error" << std::endl;
      exit(-1);
    }
    return fd;
  }

  void truncate(int fd)
  {
    const int status = ftruncate(fd, static_cast<off_t>(m_size * sizeof(Node<T>)));
    if(status != 0) {
      std::cout << "ftrucate error" << std::endl;
      exit(-1);
    }
  }

  void dump(int fd)
  {
    Node<T> ** content = array();

    std::cout << "Inside dump(): " << std::endl;
    for(unsigned i = 0; i < m_size; ++i)
    {
      std::cout << "value: " << (content[i])->value << "\t| self: " << (content[i])->self << "\t| next: " << (content[i])->next << std::endl;
    }

    Node<T> * shared = (Node<T> *) mmap(0, m_size * sizeof(Node<T>), PROT_WRITE, MAP_SHARED, fd, 0);
    if(shared == MAP_FAILED)
    {
      std::cout << "mmap failed" << std::endl;
      exit(-1);
	  }
    for(unsigned i = 0; i < m_size; ++i) 
    {
      memcpy(shared + i, content[i], sizeof(Node<T>));
    }
    std::cout << "Shared: " << std::endl;
    for(unsigned i = 0; i < m_size; ++i)
    {
      std::cout << "value: " << shared[i].value << "\t| self: " << shared[i].self << "\t| next: " << shared[i].next << std::endl;
    }
    if(munmap(shared, static_cast<off_t>(m_size * sizeof(Node<T>)))) 
    {
      std::cout << "unmap error" << std::endl;
      exit(-1);
	  }
  }

  Node<T> ** array()
  {
    Node<T> ** dumped = new Node<T> * [m_size];
    Node<T> * iter = head;
    int el = 0;
    while(iter != nullptr) {
      dumped[el] = iter;
      iter = iter->next;
      ++el;
    }
    return dumped;
  }

  Node<T> * readFromDb(const int fd)
  {
    struct stat file_statistics;
    fstat(fd, &file_statistics);
    const int noOfRecords = file_statistics.st_size / sizeof(Node<T>);
    printf("Found %d records in the file\n", noOfRecords);
    std::cout << "Size of Node<T> = " << sizeof(Node<T>) << std::endl;
    Node<T> * shared = (Node<T> *) mmap(0, file_statistics.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if(shared == MAP_FAILED)
    {
      std::cout << "mmap failed" << std::endl;
      exit(-1);
	  }
    std::cout << "Size of Node<T> with ptrs = " << shared[0].next - shared[0].self << std::endl;
    std::cout << "In deserialize():" << std::endl;
    for(int i = 0; i < noOfRecords; ++i)
    {
      std::cout << "value: " << shared[i].value << "\t | self: " << shared[i].self << "\t | next: " << shared[i].next << std::endl;
    }

    return shared;
  }

};



#endif
