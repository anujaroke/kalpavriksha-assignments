#include <stdio.h>
#include <string.h>
#include <ctype.h>


float calculate_subtotal(float ticket_price, int ticket_count){
  
  return ticket_count * ticket_price;

}

float calculate_discount(float subtotal){

  return subtotal * 0.1;

}

int is_valid_name(const char name[]) {
    if (name[0] == '\0') {
        return 0;
    }

    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha((unsigned char)name[i])) {
            return 0;
        }
    }

    return 1;
}


int main(){

  const float STANDARD_PRICE = 150.0f;
  const float PREMIUM_PRICE = 250.0f;
  const float RECLINER_PRICE = 400.0f;

  char name[50];
  int number_of_tickets;
  int category_choice;
  char category[9];
  float subtotal;
  float ticket_price;
  float discount;
  float final_bill;

  scanf("%49s", name);

  if (!is_valid_name(name)) {
    printf("Invalid name\n");
    return 1;
  }

  if (scanf("%d", &number_of_tickets) != 1) {
    printf("Invalid ticket count.");
    return 1;
  }

  if (number_of_tickets < 1 || number_of_tickets > 20){

    printf("Invalid ticket count.");
    return 1;

  }


  if (scanf("%d", &category_choice) != 1) {
    printf("Invalid category.\n");
    return 1;
  }

  switch(category_choice){
    case 1:
      ticket_price =  STANDARD_PRICE;
      strcpy(category, "Standard");
      break;
    
    case 2:
      ticket_price = PREMIUM_PRICE;
      strcpy(category, "Premium");
      break;
    
    case 3:
      ticket_price = RECLINER_PRICE;
      strcpy(category, "Recliner");
      break;

    default:
      printf("Invalid Input");
      return 1;
  }

  subtotal = calculate_subtotal(ticket_price, number_of_tickets);

  if (number_of_tickets >= 5){

    discount = calculate_discount(subtotal);

  }

  else{

    discount = 0;

  }

  final_bill = subtotal - discount;

  printf("Customer: %s", name);
  printf("\nTicket Category: %s", category);
  printf("\nTickets: %d", number_of_tickets);
  printf("\nSubtotal: %.2f", subtotal);
  printf("\nDiscount: %.2f", discount);
  printf("\nFinal Bill: %.2f", final_bill);

  return 0;
} 