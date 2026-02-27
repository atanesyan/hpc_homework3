#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define S 5

pthread_barrier_t barrier;
float temperatures[S];

void* sensorTask(void* arg) {
  int id = (int)(long)arg;

  float temp = (float)(rand() % 150) / 10.0 + 20.0;
  temperatures[id] = temp;
  printf("Sensor %d: Collected %.1f°C\n", id, temp);

  pthread_barrier_wait(&barrier);

  printf("Sensor %d: Now processing...\n", id);

  return NULL;
}

int main() {
  pthread_t sensors[S];
  float total = 0.0;
  srand(time(NULL));

  pthread_barrier_init(&barrier, NULL, S + 1);

  printf("Sensors start to collect data.\n");
  for (long i = 0; i < S; ++i) {
    pthread_create(&sensors[i], NULL, sensorTask, (void*)i);
  }

  pthread_barrier_wait(&barrier);

  for (int i = 0; i < S; ++i) {
    total += temperatures[i];
  }
  printf("All sensors collected data. Average Temperature: %.2f°C\n", total / S);

  for (int i = 0; i < S; ++i) {
    pthread_join(sensors[i], NULL);
  }

  pthread_barrier_destroy(&barrier);

  return 0;
}
