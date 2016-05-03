#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   
#include <sys/ioctl.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

//prototypes
int getFileSize(int fd);
char * loadFile(int fd, int filesize);
int parseLines(char *filebuf, int filesize); 

int main(int argc, char **argv) 
{
  int thepipe[2]; /* Parent read child write pipe */
  char buf[1000];
	int i;
  int pid;
//  int num_children = 0;

  if ( pipe( thepipe ) == -1 )
  {    // get a pipe
      printf("Cannot get a pipe");
      exit(1);
  }
	for(i = 1; i < argc; i++)
	{
    // now we have a pipe, now let's get two processes 
    if ( (pid = fork()) == -1 )
    {     
      printf("Cannot fork");
      exit(2);
    }
    // parent will exec av[2] and read from pipe
    if ( pid > 0 ){ 
      continue;
      }  
    else{

      close(thepipe[0]);       
      if ( dup2(thepipe[1], 1) == -1 )
      {
        printf("could not redirect stdin");
        exit(3);
      }
      close(thepipe[1]);        // stdin is duped, close pipe
      execl("lc1", "lc1", argv[i], (char * ) NULL);
      exit(0);
      //print_data(int* argc);
    }
  }
  close(thepipe[1]);
  if ( dup2(thepipe[0], 0) == -1 )
  {
    printf("could not redirect stdout");
    exit(4);
  }
  close(thepipe[0]);
  int total_lines = 0;
  int bytes = read(0, &buf, sizeof(buf));
  while(bytes  > 0)
  {
    buf[bytes] = 0;
    printf("%s",buf);
    char *token = strtok(buf," ");
    while(token != NULL)
    {
      if(atoi(token) == 0)
      {
        token = strtok(NULL," ");
        continue;
      }
      total_lines += atoi(token);
      token = strtok(NULL," ");
    }

    //printf("TOKEN %d: %s \n",j,token);
    //total_lines += atoi(token);
    bytes = read(0, &buf, sizeof(buf));
  }
  printf("TOTAL LINES: %d\n",total_lines);
}
