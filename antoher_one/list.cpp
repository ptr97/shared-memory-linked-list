#include <iostream>
#include <cstring>


static long long offset;

template<class T>
class pointer {
public:
  pointer(T* p) {
    m_ptr_offset = (long long) p - offset;
  }

  T& operator->() {
    return *reinterpret_cast<T*>(offset + m_ptr_offset);
  }
  
  operator T* () {
    return reinterpret_cast<T*>(offset + m_ptr_offset);
  }
  
private:
  long long m_ptr_offset;
};


char memblockWrite[1024];

char memblockRead[1024];

int main() {
  offset = 0;
  pointer<int> m9 = new (memblockWrite) int(9);
  pointer<int> m787 = new (memblockWrite + sizeof(int)) int(787);
  std::cout << *m9 << " " << *m787 << std::endl;

  // identical to detached/attached shm
  memcpy(memblockRead, memblockWrite, 1024);
  offset = memblockWrite - memblockWrite;
  
  pointer<int> a = (int*) memblockRead;
  pointer<int> b = (int*) (memblockRead + sizeof(int));
  std::cout << *a << " " << *b << std::endl;  
}
