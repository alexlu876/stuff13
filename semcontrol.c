#include "headers.h"


char * get_story() { //returns entire story
  int fd = open("story.txt", O_RDONLY);
  if (fd == -1) {
    printf("No story found. Create using -c\n");
    exit(1);
  }
  int size = lseek(fd, 0, SEEK_END);
  //printf("%d\n", size);
  if (size == -1) {
    printf("%s\n", strerror(errno));
  }
  lseek(fd, 0, SEEK_SET); //reset pointer to beginning
  char * text = malloc(size);
  read (fd, text, size);
  //printf("Text: %s\n", text);
  return text;
}



int main(int argc, char* argv[]){
  int semid, fd, shmid;
  union semun val;
  void *shm;


  int i = 0;
  while(argv[i]){

//============================Creation============================================

  if(!(strcmp(argv[i], "-c"))){
    semid = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0600); //create semaphore
    if(semid == -1){
      printf("Silly you, a story already exists!\n");
      return 0;
    }
    else{
      val.val = 1;
      printf("Created semaphore. Key: %d\t Value: %d\n", KEY, semctl(semid, 0, SETVAL, val)); //set semaphore value
      shmid = shmget(KEY, sizeof(int), IPC_CREAT | 0666); //create shared memory
      if (shmid == -1) { // if it failed
        printf("Bad shared memory: %s\n", strerror(errno));
        exit(1);
      }
      fd = creat("story.txt", 0644); //create file
    }
  }

//============================Viewing=============================================

  if(!(strcmp(argv[i], "-v"))){
    char * text = get_story();
    printf("\nEntire story:\n%s\n", text);
    free(text);
  }

//============================Removing============================================

  if(!(strcmp(argv[i], "-r"))){
    semid = semget(KEY, 1, 0);
    int semcheck = semctl(semid, 0, IPC_RMID); //remove semaphore
    if (semcheck == -1) {
      printf("No story to remove\n");
      exit(1);
    }
    printf("Removed semaphore\n");

    shmid = shmget(KEY, sizeof(int), 0);
    int shmcheck = shmctl(shmid, IPC_RMID, 0); //remove shared memory
    if (shmcheck == -1) {
      printf("Couldn't remove shared memory: %s\n", strerror(errno));
      exit(1);
    }
    printf("Removed shared memory\n");
    char * text = get_story();
    printf("\nEntire story:\n%s\n", text);
    free(text);
    remove("story.txt"); //remove story
  }

  i++;

  }
  return 0;
}
