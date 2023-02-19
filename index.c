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

USER update_user_amount(USER *user, int amount) {
  user->amount = amount;
  return *user;
}

USER adjust_user_amount(USER *user, int amount) {
  user->amount += amount;
  return *user;
}

USER add_user_to_db(USER *user) {
  FILE* fp = fopen("./database.csv", "a+");

    if (!fp) {
    printf("Error! We are not able to find the database.");
    return *user;
  }

  fprintf(fp, "%s,%s,%s,%d\n", user->email, user->password, user->name, user->amount);
  fclose(fp);

  return *user;
}

USER get_user_from_csv_line(char line[400], USER *user) {
  char line_copy[400];
  strcpy(line_copy, line);
  char* value = strtok(line_copy, ", ");
  char name[31];
  char password[31];
  char email[321];
  int amount;
  int column = 0;

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
        break;
      }
      value = strtok(NULL, ", ");
      column++;
  }
  
  update_user(email, password, name, user);
  update_user_amount(user, amount);

  return *user;
}

char get_csv_line_from_user(char *line, USER *user) {
  snprintf(line, 400, "%s,%s,%s,%d\n", user->email, user->password, user->name, user->amount);
  return *line;
}

USER get_user(char *email_input, USER *user) {
  FILE* database = fopen("./database.csv", "r");

  if (!database) {
    printf("Error! We are not able to find the database.");
    return *user;
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
            update_user(email, password, name, user);
            update_user_amount(user, amount)
;            return *user;
          }
          break;
      }
      value = strtok(NULL, ", ");
      column++;
    }
    printf("\n");
  }
  return *user;
}

USER update_user_amount_in_db(USER *current_user, int amount) {
  FILE *db_file;
  FILE *tmp_file;
  int current_line_ind = 0;
  db_file = fopen("./database.csv", "r");
  tmp_file = fopen("./tmp.csv", "w");
  char new_line[400];
  char current_line[400];
  USER empty_user = {"", "", "", 0};
  USER current_line_user = {"", "", "", 0};
  while(!feof(db_file)) {
    // set up variables
    current_line_ind++;
    strcpy(current_line, "\0");
    fgets(current_line, 400, db_file);

    // skip headers
    if (current_line_ind == 1) {
      fprintf(tmp_file, "%s", current_line);
      continue;
    }
 
    // get user from current line
    if (strlen(current_line) == 0) {
      update_user(empty_user.email, empty_user.password, empty_user.name, &current_line_user);
    } else {
      get_user_from_csv_line(current_line, &current_line_user);
    }

    // if email matches, update line
    if (strcmp(current_line_user.email, current_user->email) == 0) {
      update_user_amount(&current_line_user, amount);
      get_csv_line_from_user(new_line, &current_line_user);
      fprintf(tmp_file, "%s", new_line);
    } else {
      fprintf(tmp_file, "%s", current_line);
    }
  }
  fclose(db_file);
  fclose(tmp_file);
  remove("./database.csv");
  rename("./tmp.csv", "./database.csv");

  return *current_user;
}

// TODO
// use better alternative than scanf (doesn't work with spaces)

int main(void) {
  USER current_user = {"", "", "", 0};
  
  char option[5];
  char name[31] = "";
  char password[31] = "";
  char email[321] = "";
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
    update_user(email, password, name, &current_user);
    update_user_amount(&current_user, 0);
    add_user_to_db(&current_user);
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
  // user wants to add money
  if (strcmp(option, "1") == 0) {
    printf("What amount are you adding?: ");
    scanf("%s", option);
    int amount_input = atoi(option);
    adjust_user_amount(&current_user, amount_input);
    update_user_amount_in_db(&current_user, current_user.amount);
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
      amount_input = atoi(option);
    }
    adjust_user_amount(&current_user, amount_input * -1);
    printf("\nDispensing Now...\n");
    update_user_amount_in_db(&current_user, current_user.amount);
    for (int i = 0; i < amount_input; i++) {
      printf("💵");
    }
    printf("\nYour amount is now: %d\n", current_user.amount);
  }
}
