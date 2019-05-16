struct shm 
{
  char * startPtr;
  char * lastUsed;
};

void set_ptr_offset(void*);




template<typename T>
class shmptr {
  public:
  shmptr<T>( void* )
  {

  }
  operator const T*() const 
  { 
    return shm.startPtr + data_offset; 
  }

  static shmptr<T> make_new() 
  {
    data = shm.lastUsed;
    shm.lastused += sizeof(T);
  }

  int data_offset;
};

struct Node 
{
  int data;
  shmptr<Node> next;
};

struct List {
  shmptr<Node> head;
  void add( int el ) {
    shmptr<Node> = shmptr<Node>::make_new();
    // iteration and setting ptrs  
  }
};


int main() {

  void * ptr = shmat();
  set_shm_start_ptr(ptr);
  List * list = ptr;
  
  
}