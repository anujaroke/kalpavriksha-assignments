#include <stdio.h>
#include <string.h>

struct Student {

  int roll_number;
  char name[50];
  int marks[3];
  int total_marks;
  float average_mark;
  char grade;

};

void clear_input_buffer() {
    int character;

    while ((character = getchar()) != '\n' && character != EOF) {
    }
}

int get_valid_student_count() {

  int student_count;

  while (1) {

    printf("Enter the number of students: ");

    if (scanf("%d", &student_count) != 1) {
      printf("Invalid input. Please enter a number.\n");
      clear_input_buffer();
      continue;
    }

    clear_input_buffer();

    if (student_count < 1 || student_count > 100) {
      printf("Invalid number. Enter a value between 1 and 100.\n");
      continue;
    }

    return student_count;
  }
}

int get_valid_roll_number(int roll_number) {

  if (roll_number <= 0) {
    return 0;
  }

  return 1;
}

int get_valid_name(char name[]) {

  for (int character_index = 0; name[character_index] != '\0'; character_index++) {

    if (!((name[character_index] >= 'A' && name[character_index] <= 'Z') ||
          (name[character_index] >= 'a' && name[character_index] <= 'z'))) {

      return 0;
    }
  }

  return 1;
}

int calculate_total(int marks[]) {

  return marks[0] + marks[1] + marks[2];

}

float calculate_average(int total){

  return (float) total / 3;

}

char find_grade(float average){

  if (average >= 85){
    return 'A';
  }

  else if (average >= 70){
    return 'B';
  }

  else if (average >= 50){
    return 'C';
  }

  else if (average >= 35){
    return 'D';
  }

  else {
    return 'F';
  }
}

void display_performance(char grade) {

  switch (grade) {

    case 'A':
      printf("*****\n");
      break;
    
    case 'B':
      printf("****\n");
      break;

    case 'C':
      printf("***\n");
      break;
    
    case 'D':
      printf("**\n");
      break;
    
  }

}

void print_roll_numbers(struct Student students[], int student_index, int student_count){

  if (student_index >= student_count){
    return;
  }

  printf("%d ", students[student_index].roll_number);
  
  print_roll_numbers(students, student_index + 1, student_count);

}

int main() {

  int student_count = get_valid_student_count();

  struct Student students[student_count];

  char input[200];

  for (int student_index = 0; student_index < student_count; student_index++) {

    while (1) {

      printf("Enter details for student %d: ", student_index + 1);

      if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Invalid input. Please enter the student details again.\n");
        continue;
      }

      int parsed_values = sscanf(input, "%d %49s %d %d %d",
        &students[student_index].roll_number,
        students[student_index].name,
        &students[student_index].marks[0],
        &students[student_index].marks[1],
        &students[student_index].marks[2]);

      if (parsed_values != 5) {
        printf("Invalid input. Please enter the student details again.\n");
        continue;
      }

      if (!get_valid_roll_number(students[student_index].roll_number)) {
        printf("Invalid roll number. Please enter the student details again.\n");
        continue;
      }

      if (!get_valid_name(students[student_index].name)) {
        printf("Invalid name. Please enter the student details again.\n");
        continue;
      }

      if (students[student_index].marks[0] < 0 ||
          students[student_index].marks[0] > 100 ||
          students[student_index].marks[1] < 0 ||
          students[student_index].marks[1] > 100 ||
          students[student_index].marks[2] < 0 ||
          students[student_index].marks[2] > 100) {

        printf("Invalid marks. Marks must be between 0 and 100.\n");
        continue;
      }

      break;
    }

    students[student_index].total_marks = calculate_total(students[student_index].marks);

    students[student_index].average_mark = calculate_average(students[student_index].total_marks);

    students[student_index].grade = find_grade(students[student_index].average_mark);

  }

  for (int student_index = 0; student_index < student_count; student_index++) {

    printf("\nRoll: %d\n", students[student_index].roll_number);
    printf("Name: %s\n", students[student_index].name);
    printf("Total: %d\n", students[student_index].total_marks);
    printf("Average: %.2f\n", students[student_index].average_mark);
    printf("Grade: %c\n", students[student_index].grade);

    if (students[student_index].average_mark < 35) {
        continue;
    }
    
    printf("Performance: ");
    display_performance(students[student_index].grade);    

  }

  printf("\nList of Roll Numbers (via recursion): ");
  print_roll_numbers(students, 0, student_count);

  return 0;

}