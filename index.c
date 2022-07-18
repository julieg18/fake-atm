#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int MAX_EMAIL_LENGTH = 320;
int MAX_PASS_LENGTH = 30;
int MAX_NAME_LENGTH = 30;

typedef struct {
  char *email;
  char *password;
  char *name;
  int amount;
} USER;

typedef struct {
  USER user;
  bool does_user_exist;
} USER_EXISTS;

USER update_user_amount(USER *st, int amount) {
  st->amount += amount;
  return *st;
}

USER create_account(char *email, char *password, char *name, USER *current_user) {
  // add info to db
  current_user->email = malloc(MAX_EMAIL_LENGTH + 1);
  strcpy(current_user->email, email);
  current_user->password = malloc(MAX_PASS_LENGTH + 1);
  strcpy(current_user->password, password);
  current_user->name = malloc(MAX_NAME_LENGTH + 1);
  strcpy(current_user->name, name);
  return *current_user;
}

int main(void) {
  USER current_user = {"test@gmail.com", "password", "User", 5};
  int option;
  char name[31];
  char password[31];
  char email[321];
  printf("%s, %s, %s\n", current_user.email, current_user.password, current_user.name);
  printf("Welcome to @julieg18's Fake ATM\n\n");
  printf("Please login or sign up:\n\n(1) Login\n(2) Create an account\nOption: ");
  scanf("%d", &option);
  while (option != 1 && option != 2) {
    printf("%d is not an option. Please try again: ", option);
    scanf("%d", &option);
  }
  if (option == 1) {
    printf("\nThis feature is still being worked on.\nWe'll set you up with an example account for now.");
  }
  if (option == 2) {
    printf("Creating account...\nName: ");
    scanf("%s", name);
    printf("Email: ");
    scanf("%s", email);
    printf("Password: ");
    scanf("%s", password);
    printf("...\n");
    create_account(email, password, name, &current_user);
    printf("Account created! You are now logged in.\n");
  }
  printf("\nHello %s\n", current_user.name);
  printf("What task are you interested in making today?\n\n(1) Add Money\n(2) Withdraw Money\nOption: ");
  scanf("%d", &option);
  while (option != 1 && option != 2) {
    printf("%d is not an option. Please try again: ", option);
    scanf("%d", &option);
  }
  if (option == 1) {
    printf("What amount are you adding?: ");
    scanf("%d", &option);
    update_user_amount(&current_user, option);
    printf("Your amount is now: %d\n", current_user.amount);
  }
  if (option == 2) {
    printf("What amount are you withdrawing?: ");
    scanf("%d", &option);
    while (current_user.amount + option * -1 < 0) {
      printf("You do not have %d in your account. Please choose an amount below %d: ", option, current_user.amount);
      scanf("%d", &option);
    }
    update_user_amount(&current_user, option * -1);
    printf("\nDispensing Now...\n");
    for (int i = 0; i < option; i++) {
      printf("💵");
    }
    printf("\nYour amount is now: %d\n", current_user.amount);
  }
}
