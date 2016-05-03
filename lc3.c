#include <pthread.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>

//prototypes
void *lcthread(void *argv);

//globals
int total_lines = 0; 									     // Global counter
pthread_mutex_t total_line_lock = PTHREAD_MUTEX_INITIALIZER; // Protect with lock

main(int argc, char**argv)
{
	//create an array of threads
	pthread_t * t = malloc(500); 

	int index_create;
	for(index_create = 1; index_create < argc; index_create++)
	{
		pthread_create(&t[index_create], NULL, lcthread, (void *) argv[index_create]);
	}

	int index_join;
	for(index_join = 1; index_join < argc; index_join++)
	{
		pthread_join(t[index_join], NULL);
	}
	printf("TOTAL LINES: %d\n", total_lines);
}

void *lcthread(void *arg)
{
	int fd = open((char *)arg, O_RDWR);
	struct stat buf;
	fstat(fd, &buf);
	int size = buf.st_size;
	char * loadedFileBuffer = malloc(size);
	read(fd, loadedFileBuffer, size);
	int j =0;
	int linecount = 0;
	while(1)
	{
		if(loadedFileBuffer[j] == '\n')
		{
			linecount++;
		}
		else if(loadedFileBuffer[j] == '\0')
		{	
			printf("%d \t\t%s \n", linecount, (char *)arg);
			break;
		}
		j++;
	}
	pthread_mutex_lock(&total_line_lock);
	total_lines += linecount;
	pthread_mutex_unlock(&total_line_lock);
	close(fd);
}