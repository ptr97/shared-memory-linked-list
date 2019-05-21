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
    return *reinterpret_cast<T*>(shmBlock::offset + m_ptr_offset);
  }
  
  operator T* () {
    return reinterpret_cast<T*>(shmBlock::offset + m_ptr_offset);
  }

  T operator*() {
    return *reinterpret_cast<T*>(shmBlock::offset + m_ptr_offset);
  }
  
private:
  long long m_ptr_offset;
};

#endif
