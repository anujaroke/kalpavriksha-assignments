#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int ***create_matrix(int size, int **values_main){

  int ***matrix = malloc(size * sizeof(int **));
  if (matrix == NULL){
    return NULL;
  }

  int *values = malloc(size * size * sizeof(int));
  if (values == NULL){
    free(matrix);
    return NULL;
  }

  for (int index = 0; index < size; index++){
    *(matrix + index) = malloc(size * sizeof(int *));

    if (*(matrix + index) == NULL){
      for (int index2 = 0; index2 < index; index2++){
        free(*(matrix + index2));
      }

      free(values);
      free(matrix);
      return NULL;
    }
  }

  for (int row = 0; row < size; row++){
    for (int column = 0; column < size; column++){
      *(values + row * size + column) = rand() % 256;
      *(*(matrix + row) + column) = values + row * size + column;
    }
  }

  *values_main = values;
  return matrix;
}


void display_matrix(int ***matrix, int size){

  for (int row = 0; row < size; row++){
    for (int column = 0; column < size; column++){
      printf("%d ", *(*(*(matrix + row) + column)));
    }
    printf("\n");
  }
}


void rotate_matrix(int ***matrix, int size){

  for (int row = 0; row < size; row++){
    for (int column = row; column < size; column++){
      int *temp = *(*(matrix + row) + column);
      *(*(matrix + row) + column) = *(*(matrix + column) + row);
      *(*(matrix + column) + row) = temp;
    }
  }

  for (int index = 0; index < size; index++){
    int left = 0;
    int right = size - 1;

    while (left < right){
      int *temp = *(*(matrix + index) + left);
      *(*(matrix + index) + left) = *(*(matrix + index) + right);
      *(*(matrix + index) + right) = temp;

      left++;
      right--;
    }
  }
}


int main(){

  int size;
  printf("Enter matrix size (2-10): ");

  if (scanf("%d", &size) != 1){
    printf("Invalid input.\n");
    return 1;
  }

  if (size < 2 || size > 10){
    printf("Invalid Size.\n");
    return 1;
  }

  srand(time(NULL));

  int *values = NULL;
  int ***matrix = create_matrix(size, &values);

  if (matrix == NULL){
    printf("Memory allocation failed.\n");
    return 1;
  }

  printf("\nOriginal Randomly Generated Matrix:\n");
  display_matrix(matrix, size);

  rotate_matrix(matrix, size);
  printf("\nMatrix after 90 Degree Clockwise Rotation:\n");
  display_matrix(matrix, size);


  for (int index = 0; index < size; index++){
    free(*(matrix + index));
  }

  free(matrix);
  free(values);

  return 0;
}