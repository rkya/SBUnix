#include <stdlib.h>
#include <string.h>
#include <sys/kprintf.h>

static char var_name[MAX_ENV_VARIABLES][MAX_ENV_VARIABLES_SIZE];
static char var_value[MAX_ENV_VARIABLES][MAX_ENV_VARIABLES_SIZE];
static int var_count = 0;

int putenv(char *string) {
  char *name = strtok(string, "=");
  char *value = strtok(NULL, "=");

  for(int i = 0; i < var_count; i++) {
    if(!strcmp(name, var_name[i])) {
      strcpy(var_value[i], value);
      return 0;
    }
  }

  if(var_count >= MAX_ENV_VARIABLES) {
    return -1;
  }

  strcpy(var_name[var_count], name);
  strcpy(var_value[var_count], value);
  var_count++;

  return 0;
}

char * getenv(char *name) {
  for(int i = 0; i < var_count; i++) {
    if(!strcmp(name, var_name[i])) {
      return var_value[i];
    }
  }
  return NULL;
}

void e_print_all_variables() {
  for(int i = 0; i < var_count; i++) {
    kprintf("%s=%s\n", var_name[i], var_value[i]);
  }
}

/*static char environment_variables[MAX_ENV_VARIABLES][MAX_ENV_VARIABLES_SIZE];
static int environment_variables_size = 0;

*//*
 * Returns the index of address after '=' if name is found
 * @param string containing name and value of environment variable of the form 'name=value'
 * @param substring name of the environment variable
 * @return index of the address after '=' if name is found, -1 otherwise
 *//*
int check_env(char *string, char *substring) {
  int string_size = strlen(string);
  int substring_size = strlen(substring);
  for(int i = 0; i < string_size; i++) {
    int j = 0;
    for(; j < substring_size; j++) {
      if(string[i] != substring[j]) {
        break;
      }
    }
    if(j == substring_size) {
      return substring_size + 1;
    }
  }
  return -1;
}

int putenv(char *string) {
  char *name = strtok(string, "=");
  char *value = strtok(NULL, "=");

  if(name == NULL || strlen(name) == 0 || value == NULL || strlen(value) == 0) {
    return 0;
  }

  //if the variable is already set, just change the value
  for(int i = 0; i < environment_variables_size; i++) {
    int index = check_env(environment_variables[i], name);
    //index > 1 as the variable name must contain atlease 1 letter and 1 char for '='
    if(index > 1) {
      //strcpy(&environment_variables[index], value);
      int value_length = strlen(string);
      if(value_length + i > MAX_ENV_VARIABLES_SIZE - 1) {
        return 0;
      }

      int j;
      for(j = 0 ; j < value_length; j++) {
        environment_variables[index][j] = value[j];
      }
      environment_variables[index][j] = '\0';
      return 1;
    }
  }

  //cannot add environment variables anymore
  if(environment_variables_size >= MAX_ENV_VARIABLES) {
    return 0;
  }

  //copy the string in the environment variables array
  //strcpy(environment_variables[environment_variables_size], string);

  int string_length = strlen(string);
  if(string_length > MAX_ENV_VARIABLES_SIZE - 1) {
    return 0;
  }

  int i;
  for(i = 0 ; i < string_length; i++) {
    environment_variables[environment_variables_size][i] = string[i];
  }
  environment_variables[environment_variables_size][i] = '\0';

  environment_variables_size++;

  return 1;
}

char * getenv(char *name) {
  if(name == NULL || strlen(name) == 0) {
    return NULL;
  }

  //if the variable is present, just return the value's address
  for(int i = 0; i < environment_variables_size; i++) {
    int index = check_env(environment_variables[i], name);
    //index > 1 as the variable name must contain atlease 1 letter and 1 char for '='
    if(index > 1) {
      return &environment_variables[i][index];
    }
  }

  //variable not found
  return NULL;
}*/

void e_init_environment() {
  putenv("HOME=/");
}