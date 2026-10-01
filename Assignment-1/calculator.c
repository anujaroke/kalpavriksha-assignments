#include <stdio.h>
#include <ctype.h>

int main()
{
  char expression[100];
  printf("Enter the expression:\n");
  fgets(expression, sizeof(expression), stdin);
  
  int result = 0;
  int last_num = 0;
  int current_num = 0;
  char prev_operator = '+';
  int i = 0;

  while (expression[i] != '\0') {

    if (!isdigit(expression[i])) {

      printf("Error: Invalid expression\n");
      return 0;

    }

    current_num = 0;

    while (isdigit(expression[i])) {

        current_num = current_num * 10 + (expression[i] - '0');
        i++;
        
    }

    while (isspace(expression[i])) {

      i++;

    }

    if(expression[i] != '+' && expression[i] != '-' && expression[i] != '*' && expression[i] != '/' && expression[i] != '\0') {
      
      printf("Error: Invalid expression\n");
      return 0;

    }

    if (expression[i] == '+' || expression[i] == '-' ||
    expression[i] == '*' || expression[i] == '/') {
      
      if (prev_operator == '+') {
        result = result + last_num;
        last_num = current_num;
      }

      else if (prev_operator == '-') {
        result = result + last_num;
        last_num = -current_num;
      }

      else if (prev_operator == '*') {
        last_num = last_num * current_num;
      }

      else if (prev_operator == '/') {

        if (current_num == 0) {
          printf("Error: Division by zero\n");
          return 0;
        }

        else {
          last_num = last_num / current_num;
        }
      
      }
    
      prev_operator = expression[i];
      current_num = 0;
      i++;

      while (isspace(expression[i])) {
        i++;
      }

      if (expression[i] == '\0') {
        printf("Error: Invalid expression\n");
        return 0;
      }

    }

  }

  if (prev_operator == '+') {
    result = result + last_num;
    last_num = current_num;
  }

  else if (prev_operator == '-') {
    result = result + last_num;
    last_num = -current_num;
  }


  else if (prev_operator == '*') {
    last_num = last_num * current_num;
  }
  
  else if (prev_operator == '/') {

    if (current_num == 0) {
      printf("Error: Division by zero\n");
      return 0;
    }

    else {
      last_num = last_num / current_num;
    }

  }
  
  result = result + last_num;
  printf("Answer: %d", result);

}
