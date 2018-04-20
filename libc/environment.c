#include <stdlib.h>
#include <string.h>

#define MAX_ENV_VARIABLES 64
#define MAX_ENV_VARIABLES_SIZE 128
static char var_name[MAX_ENV_VARIABLES][MAX_ENV_VARIABLES_SIZE];
static char var_value[MAX_ENV_VARIABLES][MAX_ENV_VARIABLES_SIZE];
static int var_count = 0;

int putenv(char *string) {
  char *name = strtok(string, "=");
  char *value = strtok(NULL, "=");

  for(int i = 0; i < var_count; i++) {
    if(!strcmp(name, var_name[i])) {
      strcpy(var_value[i], value);
      return 1;
    }
  }

  if(var_count >= MAX_ENV_VARIABLES) {
    return 0;
  }

  strcpy(var_name[var_count], name);
  strcpy(var_value[var_count], value);
  var_count++;

  return 1;
}

char * getenv(char *name) {
  for(int i = 0; i < var_count; i++) {
    if(!strcmp(name, var_name[i])) {
      return var_value[i];
    }
  }
  return NULL;
}

void e_init_environment() {
  putenv("HOME=/");
}