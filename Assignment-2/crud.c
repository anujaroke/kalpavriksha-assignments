#include <stdio.h>
#include <string.h>

struct User {
  int id;
  char name[50];
  int age;
};


void clear_input_buffer() {
  int ch;
  while ((ch =  getchar()) != '\n' && ch != EOF){

  }
}

void add_user() {

  FILE *file = fopen("users.txt", "a");
  if (file == NULL) {
    printf("Error: File hasn't been opened properly\n");
    return 0;
  }
  struct User user;

  printf("Enter ID: ");
  scanf("%d", &user.id);
  clear_input_buffer();

  printf("Enter name: ");
  fgets(user.name, sizeof(user.name), stdin);
  user.name[strcspn(user.name, "\n")] = '\0';

  printf("Enter your age: ");
  scanf("%d", &user.age);
  clear_input_buffer();

  fprintf(file, "%d, %s, %d", user.id, user.name, user.age);
  fclose(file);
  
}