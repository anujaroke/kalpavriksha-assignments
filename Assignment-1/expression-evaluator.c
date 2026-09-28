#include <stdio.h>
#include <ctype.h>

int main()
{
  char expression[100];
  fgets(expression, sizeof(expression), stdin);
  int result = 0;
  int last_num = 0;
  int current_num = 0;
  char prev_operator = '+';
  for (int i=0;expression[i] != '\0';i++) {
    char c = expression[i];

    if (isdigit(c)) {
      current_num = current_num * 10 + (c - '0');

    }

    else if (isspace(c)) {
      continue;
    }

    else if(c == '+' || c == '-' || c == '*' || c == '/') {

      if (prev_operator == '+') {
        result = result + last_num;
        last_num = current_num;
      }

      else if (prev_operator == '-') {
        result = result - last_num;
        last_num = current_num;
      }

      else if (prev_operator == '*') {
        last_num = last_num * current_num;
      }

      else if (prev_operator == '/') {

        if (current_num == 0) {
          printf("Error: Division by zero.\n");
          return 0;
        }

        else {
          last_num = last_num / current_num;
        }
      
      }
    
    prev_operator = c;
    current_num = 0;

    }

    else {
      printf("Error: Invalid expression.\n");
      return 0;
    }
  
  }

  if (prev_operator == '+') {
    result = result + last_num;
    last_num = current_num;
  }

  else if (prev_operator == '-') {
    result = result - last_num;
    last_num = current_num;
  }

  else if (prev_operator == '*') {
    last_num = last_num * current_num;
  }

  else if (prev_operator == '/') {

    if (current_num == 0) {
      printf("Error: Division by zero.\n");
      return 0;
    }

    else {
      last_num = last_num / current_num;
    }
  }

  result = result + last_num;

  printf("Calculated Answer: %d", result);
  
  return 0;
}