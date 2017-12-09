#include "headers.h"

int main() {

//============================Initialization=========================================

  int semid, fd, shmid;
  int *line_size;
  struct sembuf semtake = {.sem_num = 0, .sem_op = -1, .sem_flg = SEM_UNDO}; //init struct for taking spot
  struct sembuf semrelease = {.sem_num = 0, .sem_op = 1, .sem_flg = SEM_UNDO}; //init struct for giving up spot
  semid = semget(KEY, 1, 0);
  if (semid == -1) {
    printf("Initialize using ./control -c\n");
    exit(1);
  }
  printf("wait... |\n");
  //fflush(stdout);
  semop(semid, &semtake, 1); //decrease semaphore before accessing resources

//============================Accessing data========================================

  fd = open("story.txt", O_RDWR | O_APPEND);
  shmid = shmget(KEY, sizeof(int), 0);
  line_size = shmat(shmid, 0, 0);
  //printf("%d\n", *line_size);
  //*line_size = 0;
  char *last_line = calloc(1, *line_size);
  lseek(fd, -1 * *line_size, SEEK_END); //set file offset to beginning of last line
  read(fd, last_line, *line_size);
  printf("\nLast addition: \n%s\n", last_line);
  free(last_line);
  //note: could use pread

//============================User Input============================================

  printf("\nYour addition:\n");
  char userin[10000];
  fgets(userin, 10000, stdin);
  *line_size = strlen(userin); //set shared memory to size of input line
  //printf("%lu\n", sizeof(userin));
  write(fd, userin, *line_size);

//============================Closing===============================================

  shmdt(line_size);
  semop(semid, &semrelease, 1); //increase semaphore when finished

  return 0;
}
