#include <stdio.h>
#include <stdlib.h>

void increaseCount(int* n, int* prev, int* current) {
  if (*current > *prev) {
    (*n)++;
  }
}

int main() {
  char buff[255];
  int n = -1;
  int prev = 0;
  int current = 0;

  int nSums = 0;
  int firsts[3] = {0, 0, 0};

  FILE *f = fopen("./inputs/day1.txt", "r");
  int iFillFirst=0;
  int i = -3;
  while (!feof(f)) {
    fscanf(f, "%s", buff);
    current = atoi(buff);
    increaseCount(&n, &prev, &current);
    if (i >= 0) {
      increaseCount(&nSums, &firsts[i], &current);
    }
    firsts[iFillFirst++] = current;
    if (iFillFirst == 3) {
      iFillFirst = 0;
    }
    i++;
    if (i == 3) {
      i = 0;
    }
    prev = current;
  }
  printf("%d\n", n);
  printf("%d\n", i == 0 || i == 1 ? nSums-(i+1) : nSums);
  return 0;
}
