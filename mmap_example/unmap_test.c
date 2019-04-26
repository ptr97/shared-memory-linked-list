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


int main() 
{
	const int fd = open("beatles.db", O_RDONLY, S_IRUSR | S_IWUSR );
	if( fd < 0 ) {
		perror("open");exit(-1);
	}

	struct stat file_statistics;
	fstat(fd, &file_statistics);
	const int n_records = file_statistics.st_size / sizeof(struct Person);
	printf("Found %d records in the file\n", n_records);


	struct Person * beatles = mmap(0, file_statistics.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if(beatles == MAP_FAILED){
		perror("map failed");
		exit(-1);
	}

	for(int r = 0; r < n_records; ++r) {
		printf("Adress %d: %p\n", r, beatles + r);
	}

	for(int r = 0; r < n_records; ++r) {
		printf("First name: %s Last name: %s Age: %d Next one -> %p\n",
			(beatles + r)->firstname, (beatles + r)->surname, (beatles + r)->age, (beatles + r)->next);
		printf("From pointer\n");
		printf("First name: %s Last name: %s Age: %d\n\n\n",
			((beatles + r)->next)->firstname, ((beatles + r)->next)->surname, ((beatles + r)->next)->age);
	}

	if(munmap(beatles, file_statistics.st_size)) {
		perror("unmap");
		exit(-1);
	}
	close(fd);
}
