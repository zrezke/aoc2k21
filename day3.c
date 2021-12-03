#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int* get_file_meta(char* filename) {
  FILE *f = fopen(filename, "r");
  int *arr = (int*) malloc(2*sizeof(int));
  int n_lines = 0;
  int n_chars_in_line = -1;
  char ch = '0';
  while(!feof(f))
  {
    ch = fgetc(f);
    if (!n_lines) {
      n_chars_in_line++;
    }
    if(ch == '\n')
    {
      n_lines++;
    }
  }
  fclose(f);
  *arr = n_lines;
  *(arr+sizeof(int)) = n_chars_in_line;
  return arr;
}

int get_power_consumption(int gamma_arr[], int* n_lines, int* line_width) {
  unsigned int gamma = 0;
  unsigned int epsilon = 0;
  int half_lines = *n_lines / 2;
  for (int i=0; i<*line_width; i++) {
    gamma = gamma << 1;
    epsilon = epsilon << 1;
    if (gamma_arr[i] > half_lines) {
      gamma = gamma | 0b1;
    }
    else {
      epsilon = epsilon | 0b1;
    }
  }
  //epsilon = ~gamma; Would be nice but...
  return gamma*epsilon;
}

typedef struct array_2d {
  int* content;
  int height;
  int width;
} array_2d;


array_2d new_2d(int H, int W, int* array) {
  int* cpy = (int*) malloc(H*W*sizeof(int));
  for (int i=0; i<H; i++) {
    for (int j=0; j<W; j++) {
      *(cpy+i*W+j) = *(array+i*W+j);
    }
  }
  array_2d a_2d;
  a_2d.content = cpy;
  a_2d.height = H;
  a_2d.width = W;
  return a_2d;
}
array_2d a2d_remove_rows(int H, int W, int* array, int rows[], int n) {
  int real_h = H-n;
  int* cpy = (int*) malloc(real_h*W*sizeof(int));
  int cpyI = 0;
  for (int i=0; i<H; i++) {
    uint8_t skip_iter = 0;
    for (int row=0; row<n; row++) {
      if (rows[row]==i) {
        skip_iter = 1;
        break;
      }
    }
    if (skip_iter) {
      continue;
    }
    for (int j=0; j<W; j++) {
      *(cpy+(cpyI)*W+j) = *(array+W*i+j);
    }
    cpyI++;
  }
  array_2d a_2d;
  a_2d.content = cpy;
  a_2d.height = H-n;
  a_2d.width = W;
  return a_2d;
}

int get_oxygen_generator_rating(array_2d diagnostics_data) {
  int keep = 0;
  while (diagnostics_data.height > 1) {
    if (diagnostics_data.height == 1) {
      int result = 0;
        for (int i=0; i<diagnostics_data.width; i++) {
          result = result << 1;
          result = result | *(diagnostics_data.content+i);
        }
        return result;
      }
    for (int i=0; i<diagnostics_data.width; i++) {
      keep = 0;
      for (int j=0; j<diagnostics_data.height; j++) {
        keep += *(diagnostics_data.content+i+j*diagnostics_data.width);
      }
      keep = (diagnostics_data.height - keep <= diagnostics_data.height / 2);
      int n_to_remove = 0;
      for (int j=0; j<diagnostics_data.height; j++) {
        n_to_remove += (keep != *(diagnostics_data.content+i+diagnostics_data.width*j));
      }
      int rows_to_remove[n_to_remove];
      int remove_index = 0;
      for (int j=0; j<diagnostics_data.height; j++) {
        if (keep != *(diagnostics_data.content+i+diagnostics_data.width*j)) {
          rows_to_remove[remove_index++] = j;
        }
      }

      int* p_content = diagnostics_data.content;
      diagnostics_data = a2d_remove_rows(
        diagnostics_data.height,
        diagnostics_data.width,
        p_content,
        rows_to_remove,
        n_to_remove);
      free(p_content);
    }
  }
  int result = 0;
  for (int i=0; i<diagnostics_data.width; i++) {
    result = result << 1;
    result = result | *(diagnostics_data.content+i);
  }
  return result;
}

int get_co2_scrubber_rating(array_2d diagnostics_data) {
  int keep = 0;
  while (diagnostics_data.height > 1) {
    for (int i=0; i<diagnostics_data.width; i++) {
      if (diagnostics_data.height == 1) {
          int result = 0;
        for (int i=0; i<diagnostics_data.width; i++) {
          result = result << 1;
          result = result | *(diagnostics_data.content+i);
        }
        return result;
      }
      keep = 0;
      for (int j=0; j<diagnostics_data.height; j++) {
        keep += *(diagnostics_data.content+i+j*diagnostics_data.width);
      }
      keep = ((diagnostics_data.height % 2 == 0 ? diagnostics_data.height : diagnostics_data.height+1)/2 - keep > 0);
      int n_to_remove = 0;
      for (int j=0; j<diagnostics_data.height; j++) {
        n_to_remove += (keep != *(diagnostics_data.content+i+diagnostics_data.width*j));
      }
      int rows_to_remove[n_to_remove];
      int remove_index = 0;
      for (int j=0; j<diagnostics_data.height; j++) {
        if (keep != *(diagnostics_data.content+i+diagnostics_data.width*j)) {
          rows_to_remove[remove_index++] = j;
        }
      }
      int* p_content = diagnostics_data.content;
      diagnostics_data = a2d_remove_rows(
        diagnostics_data.height,
        diagnostics_data.width,
        p_content,
        rows_to_remove,
        n_to_remove);

      free(p_content);
    }
  }
  int result = 0;
  for (int i=0; i<diagnostics_data.width; i++) {
    result = result << 1;
    result = result | *(diagnostics_data.content+i);
  }
  return result;
}

int main() {
  char* filename = "./inputs/day3.txt";
  int *file_meta = get_file_meta(filename);
  int n_lines = *file_meta;
  int line_width = *(file_meta+sizeof(int));
  free(file_meta);
  int gamma_arr[line_width];
  int diagnostic_data[n_lines][line_width];
  for (int i=0; i<line_width; i++) {
    gamma_arr[i] = 0;
  }
  char chr = '0';

  FILE *f = fopen(filename, "r");
  int i = 0;
  int line = 0;
  while (!feof(f)) {
    chr = fgetc(f);
    if (chr == EOF) {
      break;
    }
    if (chr == '\n') {
      i = 0;
      line++;
      continue;
    }
    diagnostic_data[line][i] = chr - '0';
    gamma_arr[i++] += chr - '0';
  }
  printf("POWER_CONSUMPTION: %d\n", get_power_consumption(gamma_arr, &n_lines, &line_width));
  array_2d cpy1 = new_2d(n_lines, line_width, *diagnostic_data);
  array_2d cpy2 = new_2d(n_lines, line_width, *diagnostic_data);
  int oxygen_generator_rating = get_oxygen_generator_rating(cpy1);
  int co2_scrubber_rating = get_co2_scrubber_rating(cpy2);
  printf("LIFE SUPPORT RATING: %d\n", oxygen_generator_rating * co2_scrubber_rating);
}
