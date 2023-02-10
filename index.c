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

USER update_user(char *email, char *password, char *name, USER *current_user) {
  current_user->email = malloc(MAX_EMAIL_LENGTH + 1);
  strcpy(current_user->email, email);
  current_user->password = malloc(MAX_PASS_LENGTH + 1);
  strcpy(current_user->password, password);
  current_user->name = malloc(MAX_NAME_LENGTH + 1);
  strcpy(current_user->name, name);
  return *current_user;
}

USER add_user_to_db(char *email, char *password, char *name, int amount, USER *current_user) {
  FILE* fp = fopen("./database.csv", "a+");

    if (!fp) {
    printf("Error! We are not able to find the database.");
    return *current_user;
  }

  fprintf(fp, "%s,%s,%s,%d\n", email, password, name, amount);
  fclose(fp);

  update_user(email, password, name, current_user);

  return *current_user;
}

USER get_user(char *email_input, USER *current_user) {
  FILE* database = fopen("./database.csv", "r");

  if (!database) {
    printf("Error! We are not able to find the database.");
    return *current_user;
  }

  char buffer[1024];
  int row = 0;
  int column = 0;
  char name[31];
  char password[31];
  char email[321];
  int amount;

  while (fgets(buffer, 1024, database)) {
    column = 0;
    row++;
    if (row == 1)
      continue;
    char* value = strtok(buffer, ", ");
    while (value) {
      switch(column) {
        case 0:
          strcpy(email, value);
          break;
        case 1:
          strcpy(password, value);
          break;
        case 2:
          strcpy(name, value);
          break;
        case 3:
          amount = atoi(value);
          if (strcmp(email_input, email) == 0) {
            update_user(email, password, name, current_user);
            return *current_user;
          }
          break;
      }
      value = strtok(NULL, ", ");
      column++;
    }
    printf("\n");
  }
  return *current_user;
}

USER update_user_amount(USER *st, int amount) {
  st->amount += amount;
  return *st;
}

int main(void) {
  USER current_user = {"test@gmail.com", "password", "User", 5};
  char option[5];
  char name[31] = "name";
  char password[31] = "password";
  char email[321] = "email@gmail.com";
  // welcome and show options
  printf("Welcome to @julieg18's Fake ATM\n\n");
  printf("Please login or sign up:\n\n(1) Login\n(2) Create an account\nOption: ");
  scanf("%s", option);
  while (strcmp(option, "1") != 0 && strcmp(option, "2") != 0) {
    printf("%s is not an option. Please try again: ", option);
    scanf("%s", option);
  }
  // option login was chosen
  if (strcmp(option, "1") == 0) {
    printf("Email: ");
    scanf("%s", email);
    get_user(email, &current_user);
    while (strcmp(email, current_user.email) != 0) {
      printf("Email not found. Please try again: ");
      scanf("%s", email);
    }

    printf("Password: ");
    scanf("%s", password);
    while (strcmp(password, current_user.password) != 0) {
      printf("Password is incorrect. Please try again: ");
      scanf("%s", password);
    }

    printf("You are now logged in! Welcome Back!");
  }
  // option sign up was chosen
  if (strcmp(option, "2") == 0) {
    printf("Creating account...\nName: ");
    scanf("%s", name);
    printf("Email (no need to use a real email): ");
    scanf("%s", email);
    printf("Password: ");
    scanf("%s", password);
    printf("...\n");
    add_user_to_db(email, password, name, 0, &current_user);
    printf("Account created! You are now logged in.\n");
  }
  // user can now access account
  printf("\nHello %s\n", current_user.name);
  printf("What task are you interested in making today?\n\n(1) Add Money\n(2) Withdraw Money\nOption: ");
  scanf("%s", option);
  while (strcmp(option, "1") != 0 && strcmp(option, "2") != 0) {
    printf("%s is not an option. Please try again: ", option);
    scanf("%s", option);
  }
  // user wants to add money (updates within a session, but does not update within db)
  if (strcmp(option, "1") == 0) {
    printf("What amount are you adding?: ");
    scanf("%s", option);
    int amount_input = atoi(option);
    update_user_amount(&current_user, amount_input);
    printf("Your amount is now: %d\n", current_user.amount);
  }
  // user wants to withdraw money (updates within a session, but does not update within db)
  if (strcmp(option, "2") == 0) {
    printf("What amount are you withdrawing?: ");
    scanf("%s", option);
    int amount_input = atoi(option);
    while (current_user.amount + amount_input * -1 < 0) {
      printf("You do not have %d in your account. Please choose an amount below %d: ", amount_input, current_user.amount);
      scanf("%s", option);
    }
    update_user_amount(&current_user, amount_input * -1);
    printf("\nDispensing Now...\n");
    for (int i = 0; i < amount_input; i++) {
      printf("💵");
    }
    printf("\nYour amount is now: %d\n", current_user.amount);
  }
}
