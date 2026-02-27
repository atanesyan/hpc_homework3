#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define NUM_PLAYERS 4

pthread_barrier_t barrier;

void* playerTask(void* arg) {
  int id = *(int*)arg;

  // Players connect randomly
  int delay = (rand() % 2001) + 1000;
  usleep(delay * 1000);
  printf("Player %d: Connected\n", id);

  // Getting ready simulation: delay between 1 and 3 seconds
  delay = (rand() % 2001) + 1000;
  usleep(delay * 1000);

  printf("Player %d: Ready\n", id);
  pthread_barrier_wait(&barrier);

  return NULL;
}

int main() {
  pthread_t players[NUM_PLAYERS];
  int playerIds[NUM_PLAYERS];
  srand(time(NULL));

  pthread_barrier_init(&barrier, NULL, NUM_PLAYERS);

  printf("Lobby created. Waiting for players.\n");
  for (int i = 0; i < NUM_PLAYERS; ++i) {
    playerIds[i] = i + 1;
    pthread_create(&players[i], NULL, playerTask, &playerIds[i]);
  }

  for (int i = 0; i < NUM_PLAYERS; ++i) {
    pthread_join(players[i], NULL);
  }

  printf("Game Started!\n");
  pthread_barrier_destroy(&barrier);
  
  return 0;
}
