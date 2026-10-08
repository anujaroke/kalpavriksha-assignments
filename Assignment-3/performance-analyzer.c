#include <stdio.h>

struct Student {

  int roll_number;
  char name[50];
  int marks[3];
  int total_marks;
  float average_mark;
  char grade;

};

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

  int student_count;
  printf("Enter the number of students:");
  scanf("%d", &student_count);

  struct Student students[student_count];

  for (int student_index = 0; student_index < student_count; student_index++) {

    scanf("%d %s %d %d %d", 
      &students[student_index].roll_number, 
      students[student_index].name, 
      &students[student_index].marks[0],
      &students[student_index].marks[1], 
      &students[student_index].marks[2]);

    
    students[student_index].total_marks = calculate_total(students[student_index].marks);
    students[student_index].average_mark = calculate_average(students[student_index].total_marks);
    students[student_index].grade = find_grade(students[student_index].average_mark);


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

  printf("\n List of Roll Numbers (via recursion): ");
  print_roll_numbers(students, 0, student_count);

  return 0;

}