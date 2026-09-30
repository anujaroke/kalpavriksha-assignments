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
    return;
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

  fprintf(file, "%d,%s,%d\n", user.id, user.name, user.age);
  fclose(file);
  
}


void read_users() {

  FILE *file = fopen("users.txt", "r");
  if (file == NULL) {
    printf("Error: File hasn't been opened properly\n");
    return;
  }
  struct User user;

  while (fscanf(file, "%d,%49[^,],%d", &user.id, user.name, &user.age) == 3) {

    printf("ID: %d, Name: %s, Age: %d\n", user.id, user.name, user.age);
    
  }

  fclose(file);
}


void update_user() {

  int update_id;
  printf("Enter ID to update: ");
  scanf("%d", &update_id);
  clear_input_buffer();

  FILE *file = fopen("users.txt", "r");
  if (file == NULL) {
    printf("Error: File hasn't been opened properly\n");
    return;
  }

  FILE *temp_file = fopen("temp.txt", "w");
  if (temp_file == NULL) {
    printf("Error: Temp File hasn't been opened properly\n");
    fclose(file);
    return;
  }

  struct User user;
  int found = 0;
  while (fscanf(file, "%d,%49[^,],%d", &user.id, user.name, &user.age) == 3){
    if (user.id == update_id) {
      found = 1;
      printf("User Found, What to update?\n");
      printf("1. Name\n2. Age\nEnter option: ");
      int option;
      scanf("%d", &option);
      clear_input_buffer();
      switch (option) {
        case 1:
          printf("Enter new name: ");
          fgets(user.name, sizeof(user.name), stdin);
          user.name[strcspn(user.name, "\n")] = '\0';
          break;

        case 2:
          printf("Enter new age: ");
          scanf("%d", &user.age);
          clear_input_buffer();
          break;

        default:
          printf("Invalid choice");
          break;

      }

      fprintf(temp_file, "%d,%s,%d\n", user.id, user.name, user.age);
    }

    else {

      fprintf(temp_file, "%d,%s,%d\n", user.id, user.name, user.age);

    }

  }

  if (!found) {

    printf("User not found!\n");
    fclose(file);
    fclose(temp_file);
    remove("temp.txt");
    return;

  }

  fclose(file);
  fclose(temp_file);

  if (remove("users.txt") != 0){

    printf("Error removing users.txt\n");
    return;

  }
  
  if (rename("temp.txt", "users.txt") !=0 ){

    printf("Error renaming file\n");

  }
  
}