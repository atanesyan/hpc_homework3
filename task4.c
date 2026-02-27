#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define M 4
#define STAGES 3

pthread_barrier_t pipeline_barrier;

void* workerTask(void* arg) {
  int id = (int)(long)arg;

  for (int stage = 1; stage <= STAGES; ++stage) {
    printf("Worker %d: Starting Stage %d\n", id, stage);

    // Simulate work for the current stage
    int work_time = (rand() % 1001) + 1000;
    usleep(work_time * 1000);

    printf("Worker %d: Finished Stage %d\n", id, stage);
    pthread_barrier_wait(&pipeline_barrier);
  }

  return NULL;
}

int main() {
  pthread_t workers[M];
  srand(time(NULL));

  pthread_barrier_init(&pipeline_barrier, NULL, M);

  printf("Pipeline started with %d workers.\n", M);
  for (long i = 0; i < M; ++i) {
    pthread_create(&workers[i], NULL, workerTask, (void*)i);
  }

  for (int i = 0; i < M; ++i) {
    pthread_join(workers[i], NULL);
  }

  printf("Pipeline execution complete.\n");
  pthread_barrier_destroy(&pipeline_barrier);

  return 0;
}
