#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   
#include <sys/ioctl.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

//prototypes
int getFileSize(int fd);
char * loadFile(int fd, int filesize);
int parseLines(char *filebuf, int filesize); 

int main(int argc, char **argv) {
  int totalLines = 0;

  //make sure valid file name is entered as a command line argument
  int fd;
  if ((fd = open(argv[1], O_RDWR))<0) 
  {
    printf("Missing valid textfile name in commandline argument.\n");
    exit(0);
  } else {
  	int i;
  	for(i = 1; i < argc; i++)
  	{

  		fd = open(argv[i], O_RDWR );
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
        printf("%d \t\t%s \n", linecount, argv[i]);
        break;
      }
      j++;
    }
    fflush(stdout);
    totalLines += linecount;
    close(fd);
  }
}
if (argc > 2){
  printf("total: %d \n", totalLines);
}
exit(0);
}

