#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void updateAim(int* aim, int by) {
  *aim += by;
}

void updatePosition(int* currentX, int* currentY, int* aim, int by) {
  *currentX += by;
  *currentY += *aim*by;
}

void parse_instruction(char* instruction, int scalar, int* aim, int* x, int* y) {
  if (strcmp(instruction, "up") == 0) {
    updateAim(aim, -scalar);
  }
  if (strcmp(instruction, "down") == 0) {
    updateAim(aim, scalar);
  }
  if (strcmp(instruction, "forward") == 0) {
    updatePosition(x, y, aim, scalar);
  }
}

int main() {
  char instruction [255];
  char scalar [255];
  
  int aim = 0;
  int posX = 0;
  int posY = 0;

  FILE *f = fopen("./inputs/day2.txt", "r");
  while (!feof(f)) {
    fscanf(f, "%s", instruction);
    int res = fscanf(f, "%s", scalar);
    if (res == EOF) {
      break;
    }
    parse_instruction(instruction, atoi(scalar), &aim, &posX, &posY);
  }
  printf("Horizontal: %d\tDepth: %d\n", posX, posY);
  printf("Result: %d\n", posX*posY);
}
