#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

int MAX_EMAIL_LENGTH = 320;
int MAX_PASS_LENGTH = 100;
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

USER get_user_from_csv_line(char line[500], USER *user) {
  char line_copy[500];
  strcpy(line_copy, line);
  char* value = strtok(line_copy, ", ");
  char name[31];
  char password[101];
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
  snprintf(line, 500, "%s,%s,%s,%d\n", user->email, user->password, user->name, user->amount);
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
  char password[101];
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
            update_user_amount(user, amount);
            return *user;
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
  char new_line[500];
  char current_line[500];
  USER empty_user = {"", "", "", 0};
  USER current_line_user = {"", "", "", 0};
  while(!feof(db_file)) {
    // set up variables
    current_line_ind++;
    strcpy(current_line, "\0");
    fgets(current_line, 500, db_file);

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

void get_input(char *input) {
  fgets(input, 350, stdin);
  input[strcspn(input, "\n")] = 0;
}

int get_option() {
  char input[350] = "";
  
  get_input(input);
  while (strcmp(input, "1") != 0 && strcmp(input, "2") != 0) {
    printf("%s is not an option. Please try again: ", input);
    get_input(input);
  }
  
  int opt = atoi(input);
  
  return opt;
}

int is_digits(char *string) {
  int length = strlen(string);

  for (int i = 0; i < length; i++) {
    if (isdigit(string[i]) == 0) {
      return 0;
    }
  }

  return 1;
}

int get_amount() {
  char input[350] = "";
  
  get_input(input);
  while (is_digits(input) == 0) {
    printf("%s is not a number. Please try again: ", input);
    get_input(input);
  }
  
  int amount = atoi(input);
  
  return amount;
}

int is_valid_email(char *string) {
  int length = strlen(string);
  int has_at = 0;
  int has_period = 0;
  
  if (length > 320) {
    printf("\n length is wrong...");
    return 0;
  }

  // technically you could use strstr for this logic
  // the for loop was an exercise for learning purposes :)
  for (int i = 0; i < length; i++) {
    if (string[i] == ' ') {
      return 0;
    }
    if (string[i] == '@') {
      has_at = 1;
    }
    if (string[i] == '.' && has_at) {
      has_period = 1;
    }
  }

  return (has_at && has_period) ? 1 : 0;
}

void get_email(char *email) {
  char input[350] = "";
  
  get_input(input);
  while (is_valid_email(input) == 0) {
    printf("%s is not a valid email. Please try again: ", input);
    get_input(input);
  }
  
  strcpy(email, input);
}

int is_name_valid(char *input) {
  int length = strlen(input);
  
  if (length > 30) {
    return 0;
  }
  
  for (int i = 0; i < length; i++) {
    if (input[i] == ' ') {
      return 0;
    }
  }
  return 1;
}

void get_name(char *name) {
  char input[350] = "";
  
  get_input(input);
  
  while (is_name_valid(input) == 0) {
    printf("%s is invalid. Please keep name under 30 characters with no spaces. Name:", input);
    get_input(input);
  }
  
  strcpy(name, input);
}

void get_password(char *password) {
  char input[350] = "";
  
  get_input(input);
  int length = strlen(input);
  char *has_spaces = strstr(input, " ");
  while (length > 100 || length < 15 || has_spaces != NULL) {
    printf("%s is invalid. Please don't use spaces and keep between 15-100 characters. Password:", input);
    
    get_input(input);
    length = strlen(input);
    has_spaces = strstr(input, " ");
  }
  
  strcpy(password, input);
}

int main(void) {
  USER current_user = {"", "", "", 0};
  
  // char option[5] = "";
  char name[31] = "";
  char password[101] = "";
  char email[321] = "";
  
  // welcome and show options
  printf("Welcome to @julieg18's Fake ATM\n\n");
  printf("Please login or sign up:\n\n(1) Login\n(2) Create an account\nOption: ");
  
  int opt = get_option();
  
  // option login was chosen
  if (opt == 1) {
    // get a email found in db
    printf("Email: ");
    get_email(email);
    
    get_user(email, &current_user);
    while (strcmp(email, current_user.email) != 0) {
      printf("Email not found. Please try again: ");
      get_email(email);
      get_user(email, &current_user);
    }
    
    // get password that matches in db
    char pass_input[350] = "";
    printf("Password: ");
    get_input(pass_input);
    
    while (strcmp(pass_input, current_user.password) != 0) {
      printf("Password is incorrect. Please try again: ");
      get_input(pass_input);
    }
    
     printf("You are now logged in! Welcome Back!");
  }
  
  if (opt == 2) {
    printf("Creating account...\nName (no spaces): ");
    get_name(name);
    printf("Email (no need to use a real email): ");
    get_email(email);
    printf("Password (must be at least 15 characters): ");
    get_password(password);
    printf("Loading...\n");
    update_user(email, password, name, &current_user);
    update_user_amount(&current_user, 0);
    add_user_to_db(&current_user);
    printf("Account created! You are now logged in.\n");
  }
  
  // user can now access account
  printf("\n\nHello %s! You current amount is $%d\n", current_user.name, current_user.amount);
  printf("What task are you interested in making today?\n\n(1) Add Money\n(2) Withdraw Money\nOption: ");
  opt = get_option();
  
  // user wants to add money
  if (opt == 1) {
    printf("What amount are you adding?: ");
    int amount = get_amount();
    adjust_user_amount(&current_user, amount);
    update_user_amount_in_db(&current_user, current_user.amount);
    printf("Your amount is now: $%d\n", current_user.amount);
  }
  
  // user wants to withdraw money
  if (opt == 2) {
    printf("What amount are you withdrawing?: ");
    int amount = get_amount();
    while (current_user.amount - amount < 0) {
      printf("You do not have %d in your account. Please choose an amount below %d: ", amount, current_user.amount);
      amount = get_amount();
    }
    printf("\nDispensing Now...\n");
    adjust_user_amount(&current_user, amount * -1);
    update_user_amount_in_db(&current_user, current_user.amount);
    for (int i = 0; i < amount; i++) {
      printf("💵");
    }
    printf("\nYour amount is now: $%d\n", current_user.amount);
  }
}
