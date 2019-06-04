#ifndef __SHM_PTR_H__
#define __SHM_PTR_H__


template<typename T>
class shmPtr {
public:
  shmPtr(T * p) {
    m_ptr_offset = (long long) p - shmBlock::offset;

    // std::cout << "m_ptr_offset = " << p << std::endl;
  }
  
  T& operator->() {
    // std::cout << " T& operator-> " << std::endl;
    return *reinterpret_cast<T*>(shmBlock::offset + m_ptr_offset);
  }
  
  operator T* () {
    // std::cout << " operator T* " << std::endl;
    return reinterpret_cast<T*>(shmBlock::offset + m_ptr_offset);
  }

  T& operator*() {
    // std::cout << " T operator* " << std::endl;
    return *reinterpret_cast<T*>(shmBlock::offset + m_ptr_offset);
  }

  void set(long long p) {
    m_ptr_offset = p;
  }

  bool isNull()
  {
    return (reinterpret_cast<T*>(shmBlock::offset + m_ptr_offset) == nullptr);
  }
  
private:
  long long m_ptr_offset;
};

#endif
