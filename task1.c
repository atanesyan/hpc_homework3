#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define P 4
#define R 5

int rolls[P];
int win_counts[P];
pthread_barrier_t barrier;

void* player_thread(void* arg) {
  long id = (long)arg;

  for (int r = 0; r < R; ++r) {
    rolls[id] = rand() % 6 + 1;
    pthread_barrier_wait(&barrier);
    pthread_barrier_wait(&barrier);
  }
  return NULL;
}

void find_winners(int round) {
  int max_roll = 0;
  for (int i = 0; i < P; ++i) {
    if (rolls[i] > max_roll) {
      max_roll = rolls[i];
    }
  }

  printf("Round %d: |", round);
  for (int i = 0; i < P; ++i) {
    if (rolls[i] == max_roll) {
      ++win_counts[i];
    }
    printf(" P%d : %d |", i, rolls[i]);
  }
  printf("\n");
}

int main() {
  pthread_t players[P];
  pthread_barrier_init(&barrier, NULL, P + 1);
  srand(time(NULL));

  for (long i = 0; i < P; ++i) {
    pthread_create(&players[i], NULL, player_thread, (void*)i);
  }

  for (int r = 0; r < R; ++r) {
    pthread_barrier_wait(&barrier);
    find_winners(r + 1);
    pthread_barrier_wait(&barrier);
  }

  for (int i = 0; i < P; ++i) {
    pthread_join(players[i], NULL);
  }

  int top = 0;
  for (int i = 1; i < P; ++i) {
    if (win_counts[i] > win_counts[top]) {
      top = i;
    }
  }
  printf("\nWinner: P%d (%d wins)\n", top, win_counts[top]);

  pthread_barrier_destroy(&barrier);
  return 0;
}
