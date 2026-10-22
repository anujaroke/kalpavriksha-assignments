#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

typedef struct Employee{

  int employee_id;
  char name[50];
  char designation[50];
  struct Employee *reporting_manager;
  struct Employee *reports[100];
  int report_count;

} Employee;


Employee *find_employee(Employee **employees, int employee_count, int employee_id){

  for (int index = 0; index < employee_count; index++){
    
    if (employees[index]->employee_id == employee_id){

      return employees[index];

    } 

  }

  return NULL;

}


int is_valid_id(const char *id){

  if (*id == '\0'){
    return 0;
  }

  for (int index = 0; id[index] != '\0'; index++){
    if (!isdigit(id[index])){
      return 0;
    }
  }

  long value = strtol(id, NULL, 10);
  return value > 0 && value <= INT_MAX;

}


int is_valid_name(const char *text){
  int has_letter = 0;

  for (int index = 0; text[index] != '\0'; index++){

    if (isalpha(text[index])){
      has_letter = 1;
    } 
    
    else if (text[index] != ' '){
      return 0;
    }

  }

  return has_letter;
}


int is_valid_designation(char *designation){

  if (strcasecmp(designation, "CEO") == 0){
    strcpy(designation, "CEO");
  }

  else if (strcasecmp(designation, "Department Manager") == 0){
    strcpy(designation, "Department Manager");
  }

  else if (strcasecmp(designation, "Team Lead") == 0){
    strcpy(designation, "Team Lead");
  }

  else if (strcasecmp(designation, "Senior Developer") == 0){
    strcpy(designation, "Senior Developer");
  }

  else if (strcasecmp(designation, "Developer") == 0){
    strcpy(designation, "Developer");
  }

  else if (strcasecmp(designation, "Intern") == 0){
    strcpy(designation, "Intern");
  }

  else {
    return 0;
  }

  return 1;

}


void add_employee(Employee ***employees, int *employee_count, int *capacity){

  char id_input[50];
  char manager_input[50];
  char name[50];
  char designation[50];
  int employee_id;
  int manager_id = 0;

  printf("Enter employee ID: ");

  if (scanf("%49s", id_input) != 1 || !is_valid_id(id_input)){

    printf("Invalid ID.\n");
    return;

  }

  employee_id = (int)strtol(id_input, NULL, 10);

  if (find_employee(*employees, *employee_count, employee_id) != NULL){

    printf("Employee ID already exists.\n");
    return;

  }

  printf("Enter employee name: ");

  if (scanf(" %49[^\n]", name) != 1 || !is_valid_name(name)){

    printf("Invalid name.\n");
    return;

  }

  printf("Enter designation: ");

  if (scanf(" %49[^\n]", designation) != 1 || !is_valid_designation(designation)){

    printf("Invalid designation.\n");
    return;

  }

  if (strcmp(designation, "CEO") == 0) {

    for (int index = 0; index < *employee_count; index++) {

      if (strcmp((*employees)[index]->designation, "CEO") == 0) {

        printf("A CEO already exists.\n");
        return;

      }
    }
  }

  Employee *manager = NULL;

  if (strcmp(designation, "CEO") != 0){

    printf("Enter reporting manager ID: ");

    if (scanf("%49s", manager_input) != 1){

      printf("Invalid manager ID.\n");
      return;

    }

    for (int index = 0; manager_input[index] != '\0'; index++){

      if (!isdigit(manager_input[index])){

        printf("Manager ID must contain digits only.\n");
        return;

      }
    }

    long parsed_manager_id = strtol(manager_input, NULL, 10);

    if (parsed_manager_id < 0 || parsed_manager_id > INT_MAX){
      printf("Invalid manager ID.\n");
      return;
    }

    manager_id = (int)parsed_manager_id;

    if (manager_id == 0){

      printf("A non-CEO employee must have a reporting manager.\n");
      return;

    }

    manager = find_employee(*employees, *employee_count, manager_id);

    if (manager == NULL){

      printf("Manager not found.\n");
      return;

    }

    if (manager->employee_id == employee_id){

      printf("An employee cannot report to themselves.\n");
      return;
    
    }

    if (manager->report_count >= 100){

      printf("Manager already has the maximum number of direct reports.\n");
      return;

    }
    
  }

  if (*employee_count == *capacity){
    int new_capacity = (*capacity) * 2;

    Employee **temp = realloc(*employees, new_capacity * sizeof(Employee *));

    if (temp == NULL){

      printf("Memory allocation failed.\n");
      return;

    }

    *employees = temp;
    *capacity = new_capacity;
  }

  Employee *new_employee = malloc(sizeof(Employee));

  if (new_employee == NULL){

    printf("Memory allocation failed.\n");
    return;

  }

  new_employee->employee_id = employee_id;
  strcpy(new_employee->name, name);
  strcpy(new_employee->designation, designation);
  new_employee->reporting_manager = manager;
  new_employee->report_count = 0;

  (*employees)[*employee_count] = new_employee;
  (*employee_count)++;

  if (manager != NULL){

    manager->reports[manager->report_count] = new_employee;
    manager->report_count++;

  }

  printf("Employee added successfully.\n");

}


void remove_employee(Employee **employees, int *employee_count, int employee_id){

  int employee_index = -1;

  for (int index = 0; index < *employee_count; index++){

    if (employees[index]->employee_id == employee_id){
      employee_index = index;
      break;
    }

  }

  if (employee_index == -1){
    printf("Employee not found.\n");
    return;
  }

  Employee *employee = employees[employee_index];

  if (strcmp(employee->designation, "CEO") == 0){

    printf("CEO cannot be removed.\n");
    return;

  }

  Employee *manager = employee->reporting_manager;
  Employee *new_manager = NULL;

  if (employee->report_count > 0){

    if (manager != NULL){

      for (int index = 0; index < manager->report_count; index++){

        Employee *sibling = manager->reports[index];

        if (sibling != employee && sibling->report_count + employee->report_count <= 100){
 
          if (new_manager == NULL ||
              sibling->report_count < new_manager->report_count){

            new_manager = sibling;

          }

        }

      }

    }

    if (new_manager == NULL){
      new_manager = manager;
    }

    if (new_manager == NULL){

      printf("Cannot remove employee: no manager available for their reports.\n");
      return;

    }


    int available_slots = 100 - new_manager->report_count;

    if (new_manager == manager){
      available_slots++;
    }

    if (employee->report_count > available_slots){
      printf("Cannot transfer reports: manager has insufficient capacity.\n");
      return;
    }

    for (int index = 0; index < employee->report_count; index++){

      Employee *report = employee->reports[index];

      new_manager->reports[new_manager->report_count] = report;
      new_manager->report_count++;

      report->reporting_manager = new_manager;

    }

    employee->report_count = 0;
  }

  if (manager != NULL){

    for (int index = 0; index < manager->report_count; index++){

      if (manager->reports[index] == employee){

        for (int next = index; next < manager->report_count - 1; next++){

          manager->reports[next] = manager->reports[next + 1];

        }

        manager->report_count--;
        break;

      }

    }

  }

  for (int index = employee_index; index < *employee_count - 1; index++){

    employees[index] = employees[index + 1];

  }

  (*employee_count)--;
  
  free(employee);

  printf("Employee removed successfully.\n");

}


void display_hierarchy(Employee *employee, int level){

  for (int index = 0; index < level; index++){
    printf("  ");
  }

  printf("%d - %s - %s\n", employee->employee_id, employee->name, employee->designation);

  for (int index = 0; index < employee->report_count; index++){

    display_hierarchy(employee->reports[index], level + 1);

  }

}


void display_company_hierarchy(Employee **employees, int employee_count){

  int found_root = 0;

  for (int index = 0; index < employee_count; index++){

    if (employees[index]->reporting_manager == NULL){

      display_hierarchy(employees[index], 0);
      found_root = 1;

    }

  }

  if (!found_root){

    printf("No employees to display.\n");

  }

}


int count_employees(Employee *employee){

  int count = 0;

  for (int index = 0; index < employee->report_count; index++){

    count++;

    count += count_employees(employee->reports[index]);

  }

  return count;

}


void display_company(Employee **employees, int employee_count){

  display_company_hierarchy(employees, employee_count);

  printf("\nTotal employees: %d\n", employee_count);

}


int main(){

  Employee **employees;
  int employee_count = 0;
  int capacity = 2;
  int choice;
  int employee_id;

  employees = malloc(capacity * sizeof(Employee *));
  if (employees == NULL){

    printf("Memory Allocation failed.\n");
    return 1;

  }

  while (1){

    printf("\n===== Employee Hierarchy Management =====\n");
    printf("1. Add employee\n");
    printf("2. Remove employee\n");
    printf("3. Display employee hierarchy\n");
    printf("4. Display complete company hierarchy\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");

    if (scanf("%d", &choice) != 1){

      printf("Invalid input. Enter a number.\n");

      int character;
      while ((character = getchar()) != '\n' && character != EOF);

      continue;

    }

    switch (choice){

      case 1:
        add_employee(&employees, &employee_count, &capacity);
        break;

      case 2:
        printf("Enter employee ID to remove: ");

        if (scanf("%d", &employee_id) != 1 || employee_id <= 0){

          printf("Invalid employee ID.\n");

          int character;
          while ((character = getchar()) != '\n' && character != EOF);

          break;

        }

        remove_employee(employees, &employee_count, employee_id);
        break;

      case 3:
        printf("Enter employee ID: ");

        if (scanf("%d", &employee_id) != 1 || employee_id <= 0){

          printf("Invalid employee ID.\n");

          int character;
          while ((character = getchar()) != '\n' && character != EOF);

          break;

        }

        Employee *employee = find_employee(employees, employee_count, employee_id);

        if (employee == NULL){

          printf("Employee not found.\n");

        }

        else {

          display_hierarchy(employee, 0);
          printf("\nTotal employees under %d: %d\n", employee_id, count_employees(employee));

        }

        break;

      case 4:
        display_company(employees, employee_count);
        break;

      case 5:
        for (int index = 0; index < employee_count; index++){

          free(employees[index]);

        }

        free(employees);

        printf("Exiting program.\n");
        return 0;

      default:
        printf("Invalid choice. Choose between 1 and 5.\n");

    }

  }

}