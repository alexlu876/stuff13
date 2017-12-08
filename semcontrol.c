#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#define KEY 174209


int main(int argc, char* argv[]){
  int sd;
  int i = 0;
  while(argv[i]){
  if(!(strcmp(argv[i], "-c"))){
    sd = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
    if(sd == -1){
      printf("Silly you, a semaphore already exists!\n");
      return 0;
    }
    else{
      printf("Semaphore created. Key: %d\n", KEY);
      semctl(sd, 0, SETVAL, atoi(argv[i+1]));
    }
  }
  if(!(strcmp(argv[i], "-v"))){
    printf("Semaphore value: %d\n", semctl(semget(KEY, 0, 0), 0, GETVAL));
  }
  if(!(strcmp(argv[i], "-r"))){
    semctl(semget(KEY, 0, 0), 0, IPC_RMID);
    printf("Semaphore removed\n");
  }
  i++;
  }
}






