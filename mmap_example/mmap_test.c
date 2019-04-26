#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define NSZ 32


struct Person  
{
	char firstname[NSZ]; 
	char surname[NSZ]; 
	int age;
	struct Person * next; 
};

void fill(struct Person * p, const char * first, const char * sur, int age, struct Person * ptr) 
{ 
	strncpy(p->firstname, first, sizeof(p->firstname)); 
	strncpy(p->surname, sur, sizeof (p->surname)); 
	p->age = age; 
	p->next = ptr;
}


int main() 
{
	const int fd = open("beatles.db", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR );
	if( fd < 0 ) {
		perror("open");exit(-1);
	}

	off_t DBsize = 4 * sizeof(struct Person);
	const int status = ftruncate(fd, DBsize);
	if(status != 0) {
		perror("ftrucate");
		exit(-1);
	}

	struct Person * beatles = mmap(0, DBsize, PROT_WRITE, MAP_SHARED, fd, 0);
	if(beatles == MAP_FAILED){
		perror("map failed");
		exit(-1);
	}

	fill(beatles, "John", "Lenon", 34, (beatles + 1));
	fill(beatles + 1, "Paul", "McCartney", 37, (beatles + 2));
	fill(beatles + 2, "George", "Harrison", 29, (beatles + 3));
	fill(beatles + 3, "Ringo", "Star", 34, (beatles));

	for(int i = 0; i < 4; ++i) {
		printf("Adress %d: %p\n", i, beatles + i);
	}

	if(munmap(beatles, DBsize)) {
		perror("unmap");
		exit(-1);
	}
	close(fd);
}
