#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
//#include "shmdata.h"

int main()
{
  int running = 1;
  void *shm = NULL;
  struct shared_use_st *shared;
  int shmid;
  shmid = shmget(( key_t)16857798 ,488,0644 | IPC_CREAT);
  if(shmid == -1)
  {
     fprintf(stderr,"shmget failed \n");
     exit(EXIT_FAILURE);
  }
  shm = shmat(shmid , 0 , 0 );
  if(shm == (void*)-1)
  {
    fprintf(stderr ,"shmat failer\n");
    exit(EXIT_FAILURE);
  }
  printf("\n Memory attached at %0x \n",(int)(shm));
 /* shared = (struct shared_use_st *)shm;
  shared->written = 0 ;
  while(running)
  {
    if(shared->written != 0)
    {
        printf("You wrote :%s ", shared->text);
        sleep(rand() %3);
        shared->written  = 0 ;
        if(strncmp(shared->text , "end", 3)  == 0)
            running = 0;
    }
    else
      sleep(1);
  }
  if(shmdt(shm) == -1)
  {
    fprintf(stderr,"shmct1(IPC_RMID) failed \n");
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);*/

}
