#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <test_sbush.h>
#include <sys/kprintf.h>

#define SBUSH_CMD_BUFFER 128
#define NEW_LINE 10

static FILE *stderr = NULL;
static FILE *stdin = NULL;

char **env_variables = NULL;

void sbush_export_command(char **sbush_cmd_tokens) {

  //List all the environment variables if no arguments are passed
  if(sbush_cmd_tokens[1] == NULL) {
    char **temp_env_variables;
    for(temp_env_variables = env_variables; *temp_env_variables; temp_env_variables++) {
      printf("%s\n", *temp_env_variables);
    }
  } else {
    char *delimiters = "=";
    char *env_variable_name, *env_variable_value;
    env_variable_name = strtok(sbush_cmd_tokens[1], delimiters);
    env_variable_value = strtok(NULL, delimiters);

    //return if the argument is not in a proper format
    if(!env_variable_name || !env_variable_value || env_variable_value[0] != '"' ||
      env_variable_value[strlen(env_variable_value) - 1] != '"') {
      fprintf(stderr, "sbush:error:invalid export command format \n");
      return;
    }

    //create a proper string for the putenv()
    env_variable_value[strlen(env_variable_value) - 1] = '\0';
    char *env_string = (char *)
      malloc (sizeof(char) * (strlen(env_variable_name) + strlen(env_variable_value)));
    env_string[0] = '\0';
    strcat(env_string, env_variable_name);
    strcat(env_string, "=");
    strcat(env_string, &env_variable_value[1]);
    if(putenv(env_string)) {
      fprintf(stderr, "sbush:error:putenv could not set the environment variable \n");
    } else {
      printf("New value set for %s: %s\n", env_variable_name, getenv(env_variable_name));
    }
  }
}

void sbush_cd_command(char **sbush_cmd_tokens) {
  char *home_path;
  int sbush_chdir_ret = 0;
  if(sbush_cmd_tokens[1] == NULL){
    home_path = getenv("HOME");
    sbush_chdir_ret = chdir(home_path);
  } else {
    sbush_chdir_ret = chdir(sbush_cmd_tokens[1]);
  }
  if(sbush_chdir_ret < 0){
    fprintf(stderr, "sbush:error:invalid path to directory or file \n");
  }
  //system("pwd");
}

char * sbush_get_cmd() {
  int position, cmd_buffer_size = SBUSH_CMD_BUFFER;
  char c;
  char * sbush_cmd;
  sbush_cmd = malloc(sizeof(char) * SBUSH_CMD_BUFFER);

  if(!sbush_cmd) {
    fprintf(stderr, "sbush:error:cannot allocate memory\n");
    exit(EXIT_FAILURE);
  }

  position = 0;
  while(1) {
    c = getchar();
    if(c == '\n') {
      c = '\0';
      sbush_cmd[position] = c;
      return sbush_cmd;
    } else {
      sbush_cmd[position] = c;
      position++;
    }

    if(position >= cmd_buffer_size) {
      cmd_buffer_size += SBUSH_CMD_BUFFER;
      sbush_cmd = realloc(sbush_cmd, sizeof(char) * cmd_buffer_size);

      if(!sbush_cmd){
        fprintf(stderr, "sbush:error:cannot allocate memory\n");
        exit(EXIT_FAILURE);
      }
    }
  }

}

void sbush_remove_escape_character(char *sbush_cmd) {
  char *token;
  char *delimiters = "\\";
  char *sbush_cmd_temp = malloc(sizeof(char) * strlen(sbush_cmd));

  for(token = strtok(sbush_cmd, delimiters); token; token = strtok(NULL, delimiters)) {
    strcat(sbush_cmd_temp, token);
  }
  strcpy(sbush_cmd, sbush_cmd_temp);
}

void sbush_init_tokens(char **sbush_cmd_tokens, int sbush_cmd_tokens_buffer){
  for(int i = 0; i < sbush_cmd_tokens_buffer; i++)
    sbush_cmd_tokens[i] = NULL;
}

char ** sbush_cmd_tokenize(char *sbush_cmd) {
  char *token;
  char *delimiters = " \t";
  char **sbush_cmd_tokens = NULL;
  int position;
  int sbush_cmd_tokens_buffer = SBUSH_CMD_BUFFER;

  //sbush_remove_escape_character(sbush_cmd);

  sbush_cmd_tokens = malloc(sizeof(char *) * sbush_cmd_tokens_buffer);

  if(!sbush_cmd_tokens){
    fprintf(stderr, "sbush:error:cannot allocate memory\n");
    exit(EXIT_FAILURE);
  }

  sbush_init_tokens(sbush_cmd_tokens, sbush_cmd_tokens_buffer);

  position = 0;

  for(token = strtok(sbush_cmd, delimiters); token; token = strtok(NULL, delimiters)){
    sbush_cmd_tokens[position] = token;
    position++;
    if(position >= sbush_cmd_tokens_buffer){
      sbush_cmd_tokens_buffer += SBUSH_CMD_BUFFER;
      sbush_cmd_tokens = realloc(sbush_cmd_tokens, sbush_cmd_tokens_buffer);

      if(!sbush_cmd_tokens){
        fprintf(stderr, "sbush:error:cannot allocate memory\n");
        exit(EXIT_FAILURE);
      }
    }
  }

  return sbush_cmd_tokens;
}

int sbush_is_input_empty(char *sbush_cmd) {
  int i;
  for(i = 0; i < strlen(sbush_cmd); i++){
    if( sbush_cmd[i] > 'a' || sbush_cmd[i] < 'z')
      return 0;
  }
  return 1;
}

void sbush_execute_cmd(char *sbush_cmd) {
  char **sbush_cmd_tokens;
  pid_t pid;
  int return_code;
  int start_background = 0;
  int position = 0;

  if( sbush_is_input_empty(sbush_cmd) ) {
    return;
  }

  sbush_cmd_tokens = sbush_cmd_tokenize(sbush_cmd);

  while(sbush_cmd_tokens[position] != NULL){
    if(!strcmp(sbush_cmd_tokens[position], "&")){
      start_background = 1;
      sbush_cmd_tokens[position] = NULL;
    }
    position++;
  }

  if(!strcmp(sbush_cmd_tokens[0], "cd")) {
    sbush_cd_command(sbush_cmd_tokens);
  } else if(!strcmp(sbush_cmd_tokens[0], "export")) {
    sbush_export_command(sbush_cmd_tokens);
  } else {
    pid = fork();
    if(pid == 0){
      return_code = execvp(sbush_cmd_tokens[0], sbush_cmd_tokens);
      if(return_code == -1){
        fprintf(stderr, "sbush:error:no such command:'%s'\n", sbush_cmd_tokens[0]);
      }
    } else if(pid < 0){
      fprintf(stderr, "sbush:error:Problem while forking\n");
      exit(EXIT_FAILURE);
    } else {
      if(start_background) {
        return;
      }
      wait(NULL);
    }
  }
}

char *sbush_read_file_line(FILE *fp, char *sbush_cmd) {
  char c;
  int sbush_cmd_buffer = SBUSH_CMD_BUFFER;
  int position;
  sbush_cmd = malloc(sizeof(char) * sbush_cmd_buffer);

  if(!sbush_cmd) {
    fprintf(stderr, "sbush:error:cannot allocate memory\n");
    exit(EXIT_FAILURE);
  }

  position  = 0;

  for(c = getc(fp); c != EOF; c = getc(fp)){
    if(c == '\n'){
      sbush_cmd[position] = '\0';
      return sbush_cmd;
    }
    sbush_cmd[position] = c;
    position++;

    if(position > sbush_cmd_buffer){
      sbush_cmd_buffer += SBUSH_CMD_BUFFER;
      sbush_cmd = realloc(sbush_cmd, sbush_cmd_buffer);

      if(!sbush_cmd) {
        fprintf(stderr, "sbush:error:cannot allocate memory\n");
        exit(EXIT_FAILURE);
      }
    }
  }
  return NULL;
}

void sbush_execute_script(char *argv[]) {
  FILE *fp;
  char *sbush_cmd = NULL;

  fp = fopen(argv[1], "r");

  while( (sbush_cmd = sbush_read_file_line(fp, sbush_cmd)) != NULL ){
    if(sbush_cmd[0] == '#'){
      continue;
    }
    sbush_execute_cmd(sbush_cmd);
  }
  exit(EXIT_SUCCESS);
}

int test_sbush_main() {

  kprintf("Inside test sbush\n");
  while(!feof(stdin)){
    char *sbush_cmd = NULL;
    char current_working_directory[64];

//    if(argc == 2){
//      sbush_execute_script(argv);
//    }

    if(getcwd(current_working_directory, sizeof(current_working_directory)) == NULL) {
      current_working_directory[0] = '\0';
    }
    kprintf("%s%s%s", "sbush:", current_working_directory, "$ ");

    sbush_cmd = sbush_get_cmd();
    sbush_execute_cmd(sbush_cmd);

  }
  return 0;
}

void function_cat(char *filename) {

}