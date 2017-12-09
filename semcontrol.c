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


char * get_story() {
  int fd = open("story.txt", O_RDONLY);
  if (fd == -1) {
    printf("No story found. Create using -c\n");
    exit(1);
  }
  int size = lseek(fd, 0, SEEK_END);
  if (size == -1) {
    printf("%s\n", strerror(errno));
  }
  char * text = malloc(size);
  read (fd, text, size);
  return text;
}



int main(int argc, char* argv[]){
  int semid, fd, shmid;
  union semun val;
  void *shm;


  int i = 0;
  while(argv[i]){
  if(!(strcmp(argv[i], "-c"))){
    semid = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0600); //create semaphore
    if(semid == -1){
      printf("Silly you, a semaphore already exists!\n");
      return 0;
    }
    else{
      val.val = 1;
      printf("Semaphore created. Key: %d\t Value: %d\n", KEY, semctl(semid, 0, SETVAL, val)); //set semaphore value
      shmid = shmget(KEY, sizeof(int), IPC_CREAT | 0666); //create shared memory
      if (shmid == -1) { // if it failed
        printf("Bad shared memory: %s\n", strerror(errno));
        exit(1);
      }
      shm = shmat(shmid, 0, 0); //shared memory pointer
      fd = creat("story.txt", 0644); //create file
    }
  }
  if(!(strcmp(argv[i], "-v"))){
    char * text = get_story();
    printf("Entire story:\n%s\n", text);
    free(text);
  }
  if(!(strcmp(argv[i], "-r"))){
    semid = semget(KEY, 1, 0);
    int semcheck = semctl(semid, 0, IPC_RMID); //remove semaphore
    if (semcheck == -1) {
      printf("Couldn't access semaphore: %s\n", strerror(errno));
      exit(1);
    }
    printf("Semaphore removed\n");

    shmid = shmget(KEY, sizeof(int), 0);
    int shmcheck = shmctl(shmid, IPC_RMID, 0); //remove shared memory
    if (shmcheck == -1) {
      printf("Couldn't remove shared memory: %s\n", strerror(errno));
      exit(1);
    }
    printf("Shared memory removed\n");
    char * text = get_story();
    printf("Entire story:\n%s\n", text);
    free(text);
    remove("story.txt"); //remove story
  }
  i++;

  }
  return 0;
}
