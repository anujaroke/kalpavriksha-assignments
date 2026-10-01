#include <stdio.h>
#include <ctype.h>

int main()
{
  char expression[100];
  printf("Enter the expression:\n");
  fgets(expression, sizeof(expression), stdin);
  
  int result = 0;
  int last_number = 0;
  int current_number = 0;
  char previous_operator = '+';
  int position = 0;

  while (expression[position] != '\0') {

    if (!isdigit(expression[position])) {

      printf("Error: Invalid expression\n");
      return 0;

    }

    current_number = 0;

    while (isdigit(expression[position])) {

        current_number = current_number * 10 + (expression[position] - '0');
        position++;
        
    }

    while (isspace(expression[position])) {

      position++;

    }

    if(expression[position] != '+' && expression[position] != '-' && expression[position] != '*' && expression[position] != '/' && expression[position] != '\0') {
      
      printf("Error: Invalid expression\n");
      return 0;

    }

    if (expression[position] == '+' || expression[position] == '-' ||
    expression[position] == '*' || expression[position] == '/') {
      
      if (previous_operator == '+') {
        result = result + last_number;
        last_number = current_number;
      }

      else if (previous_operator == '-') {
        result = result + last_number;
        last_number = -current_number;
      }

      else if (previous_operator == '*') {
        last_number = last_number * current_number;
      }

      else if (previous_operator == '/') {

        if (current_number == 0) {
          printf("Error: Division by zero\n");
          return 0;
        }

        else {
          last_number = last_number / current_number;
        }
      
      }
    
      previous_operator = expression[position];
      current_number = 0;
      position++;

      while (isspace(expression[position])) {
        position++;
      }

      if (expression[position] == '\0') {
        printf("Error: Invalid expression\n");
        return 0;
      }

    }

  }

  if (previous_operator == '+') {
    result = result + last_number;
    last_number = current_number;
  }

  else if (previous_operator == '-') {
    result = result + last_number;
    last_number = -current_number;
  }


  else if (previous_operator == '*') {
    last_number = last_number * current_number;
  }
  
  else if (previous_operator == '/') {

    if (current_number == 0) {
      printf("Error: Division by zero\n");
      return 0;
    }

    else {
      last_number = last_number / current_number;
    }

  }
  
  result = result + last_number;
  printf("Answer: %d", result);

}
