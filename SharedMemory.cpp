#include "SharedMemory.h"

char * shmBlock::startPtr;
char * shmBlock::lastUsed;
long long shmBlock::offset = 0;
