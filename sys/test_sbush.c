#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <dirent.h>
#include <test_sbush.h>
#include <sys/kprintf.h>
#include <sys/process.h>
#include <sys/terminal.h>
#include <sys/syscall.h>
#include <environment.h>
#include <signal.h>

#define SBUSH_CMD_BUFFER 128
#define NEW_LINE 10
#define COMMAND_MAX_LENGTH 128
#define COMMAND_MAX_ARGUMENTS 128

//static FILE *stderr = NULL;
//static FILE *stdin = NULL;

//extern char environment_variables[MAX_ENV_VARIABLES][MAX_ENV_VARIABLES_SIZE];
//extern int environment_variables_size;

char **env_variables = NULL;

void sbush_export_command(char sbush_cmd_tokens[COMMAND_MAX_ARGUMENTS][COMMAND_MAX_LENGTH]) {

  //List all the environment variables if no arguments are passed
  if(sbush_cmd_tokens[1][0] == '\0') {
    /*for(int i = 0; i < environment_variables_size; i++) {
      kprintf("%s\n", environment_variables[i]);
    }*/
    e_print_all_variables();
  } else {
    char *delimiters = "=";
    char *env_variable_name, *env_variable_value;
    env_variable_name = strtok(sbush_cmd_tokens[1], delimiters);
    env_variable_value = strtok(NULL, delimiters);

    //return if the argument is not in a proper format
    if(env_variable_name == NULL || strlen(env_variable_name) == 0 ||
      env_variable_value == NULL || strlen(env_variable_value) == 0 ) {
      fprintf(stderr, "sbush:error:invalid export command format \n");
      return;
    }

    //create a proper string for the putenv()
    /*env_variable_value[strlen(env_variable_value) - 1] = '\0';
    char *env_string = (char *)
      malloc (sizeof(char) * (strlen(env_variable_name) + strlen(env_variable_value)));
    env_string[0] = '\0';
    strcat(env_string, env_variable_name);
    strcat(env_string, "=");
    strcat(env_string, &env_variable_value[1]);
     */

    if(putenv(sbush_cmd_tokens[1])) {
      fprintf(stderr, "sbush:error:putenv could not set the environment variable \n");
    }/* else {
      kprintf("New value set for %s: %s\n", env_variable_name, getenv(env_variable_name));
    }*/
  }
}

void sbush_pwd_command(char sbush_cmd_tokens[COMMAND_MAX_ARGUMENTS][COMMAND_MAX_LENGTH]) {
  //kprintf("inside pwd\n");
  char cwd[256];
  if(getcwd(cwd, sizeof(cwd))) {
    kprintf("%s\n", cwd);
  } else {
    fprintf(stderr, "sbush:error:invalid directory \n");
  }
}

void sbush_kill_command(char sbush_cmd_tokens[COMMAND_MAX_ARGUMENTS][COMMAND_MAX_LENGTH]) {
  if(sbush_cmd_tokens[1][0] == '\0') {
    kprintf("Usage: kill pid\n");
    return;
  }

  int pid = (pid_t)stoi(sbush_cmd_tokens[2]);
  int sig = stoi(sbush_cmd_tokens[1]);
  int ret_val = kill(pid, sig);
  if(ret_val == 0) {
    kprintf("Process with pid %d killed.\n", pid);
  } else {
    kprintf("Could not kill process with pid %d.\n", pid);
  }
}

void sbush_shutdown_command(char sbush_cmd_tokens[COMMAND_MAX_ARGUMENTS][COMMAND_MAX_LENGTH]) {
  shutdownOS();
}

void sbush_cd_command(char sbush_cmd_tokens[COMMAND_MAX_ARGUMENTS][COMMAND_MAX_LENGTH]) {
  char *home_path;
  int sbush_chdir_ret = 0;
  if(!strlen(sbush_cmd_tokens[1])){
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

void sbush_ls_command(char sbush_cmd_tokens[COMMAND_MAX_ARGUMENTS][COMMAND_MAX_LENGTH]) {
  //kprintf("Inside ls command.\n");

  char ls_path[64];
  getcwd(ls_path, sizeof(ls_path));
  DIR *ls_directory = opendir(ls_path);
  struct dirent *current_dirent;

  if(ls_directory == NULL) {
    kprintf("opendir() returned NULL\n");
  } else {
    while((current_dirent = readdir(ls_directory)) != NULL) {
      kprintf("%s\n", current_dirent->d_name);
    }
    closedir(ls_directory);
  }
}

void sbush_echo_command(char sbush_cmd_tokens[COMMAND_MAX_ARGUMENTS][COMMAND_MAX_LENGTH]) {
  //kprintf("Inside echo command.\n");

  char buffer[COMMAND_MAX_LENGTH + 1];

  if(sbush_cmd_tokens[1][0] == '"') {
    int i = 1;
    int j = 1;
    int k = 0;
    int continue_flag = 1;

    while(continue_flag && i < COMMAND_MAX_ARGUMENTS) {
      while (j < COMMAND_MAX_LENGTH) {
        if(sbush_cmd_tokens[i][j] == '\0' || sbush_cmd_tokens[i][j] != '"') {
          continue_flag = 0;
          break;
        }
        buffer[k++] = sbush_cmd_tokens[i][j++];
      }
      buffer[k] = '\0';
      kprintf("%s ", buffer);
      i++;
      j = 0;
      k = 0;
    }

    int length = strlen(sbush_cmd_tokens[1]);
    if(length == 1) {
      kprintf("\n");
    } else {
      sbush_cmd_tokens[1][length - 1] = '\0';
      kprintf("%s\n", &sbush_cmd_tokens[1][1]);
    }
  } else {
    if(sbush_cmd_tokens[1][0] == '$' && sbush_cmd_tokens[1][1] != '\0') {
      char *env_value = getenv(&sbush_cmd_tokens[1][1]);
      if(env_value != NULL) {
        kprintf("%s", env_value);
      }
      kprintf("\n");
    } else {
      kprintf("%s\n", sbush_cmd_tokens[1]);
    }
  }
}

void sbush_ps_command(char sbush_cmd_tokens[COMMAND_MAX_ARGUMENTS][COMMAND_MAX_LENGTH]) {
  p_print_all_active_processes();
}

void sbush_sleep_command(char sbush_cmd_tokens[COMMAND_MAX_ARGUMENTS][COMMAND_MAX_LENGTH]) {
  int time_in_sec = stoi(sbush_cmd_tokens[1]);
  if(time_in_sec > 0) {
    sleep(time_in_sec);
  }
}

void sbush_clear_command(char sbush_cmd_tokens[COMMAND_MAX_ARGUMENTS][COMMAND_MAX_LENGTH]) {
  t_init_screen();
}

void sbush_cat_command(char sbush_cmd_tokens[COMMAND_MAX_ARGUMENTS][COMMAND_MAX_LENGTH]) {
  int fd = open(sbush_cmd_tokens[1], 1);
  char buffer[128];
  if(fd > -1) {
    while(read(fd, buffer, sizeof(buffer)) > 0) {
      kprintf("%s", buffer);
    }
    close(fd);
  } else {
    kprintf("Could not open file: %s", sbush_cmd_tokens[1]);
  }
  kprintf("\n");

  /*FILE *fp;
  char c;

  fp = fopen(sbush_cmd_tokens[1],"r");

  if(fp != NULL) {
//    while(1) {
      c = getc(fp);
//      if( feof(fp) ) {
//        break ;
//      }
      printf("%c", c);
//    }
    fclose(fp);
  } else {
    kprintf("Could not open file: %s", sbush_cmd_tokens[1]);
  }
  kprintf("\n");*/














  /*while(1) {
    c = getc(fp);
    if( feof(fp) ) {
      break ;
    }
    printf("%c", c);
  }
  fclose(fp);*/

}

char * sbush_get_cmd() {
  //int position;
  //int cmd_buffer_size = SBUSH_CMD_BUFFER;
  //char c;
  char * sbush_cmd;
  sbush_cmd = malloc(sizeof(char) * SBUSH_CMD_BUFFER);

  if(!sbush_cmd) {
    fprintf(stderr, "sbush:error:cannot allocate memory\n");
    exit(EXIT_FAILURE);
  }

  gets(sbush_cmd);
  return sbush_cmd;

  /*position = 0;
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
  }*/

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

void sbush_init_tokens(char sbush_cmd_tokens[COMMAND_MAX_ARGUMENTS][COMMAND_MAX_LENGTH], int sbush_cmd_tokens_buffer){
  for(int i = 0; i < COMMAND_MAX_ARGUMENTS; i++)
    sbush_cmd_tokens[i][0] = '\0';
}

void sbush_cmd_tokenize(char sbush_cmd_tokens[COMMAND_MAX_ARGUMENTS][COMMAND_MAX_LENGTH], char *sbush_cmd) {
  char *token;
  char *delimiters = " \t\n";
  int position;
  int sbush_cmd_tokens_buffer = SBUSH_CMD_BUFFER;

  //sbush_remove_escape_character(sbush_cmd);

  //sbush_cmd_tokens = malloc(sizeof(char *) * sbush_cmd_tokens_buffer);

  if(!sbush_cmd_tokens){
    fprintf(stderr, "sbush:error:cannot allocate memory\n");
    exit(EXIT_FAILURE);
  }

  sbush_init_tokens(sbush_cmd_tokens, sbush_cmd_tokens_buffer);

  position = 0;

  for(token = strtok(sbush_cmd, delimiters);
      strlen(token) > 0 && position < COMMAND_MAX_ARGUMENTS;
      token = strtok(NULL, delimiters)){
    strcpy(sbush_cmd_tokens[position], token);
    position++;
    /*if(position >= sbush_cmd_tokens_buffer){
      sbush_cmd_tokens_buffer += SBUSH_CMD_BUFFER;
      sbush_cmd_tokens = realloc(sbush_cmd_tokens, sbush_cmd_tokens_buffer);

      if(!sbush_cmd_tokensr){
        fprintf(stderr, "sbush:error:cannot allocate memory\n");
        exit(EXIT_FAILURE);
      }
    }*/
  }

  //return sbush_cmd_tokens;
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
  char sbush_cmd_tokens[COMMAND_MAX_ARGUMENTS][COMMAND_MAX_LENGTH];
  pid_t pid;
//  int return_code;
  int start_background = 0;
  int position = 0;

  if( sbush_is_input_empty(sbush_cmd) ) {
    return;
  }

  sbush_cmd_tokenize(sbush_cmd_tokens, sbush_cmd);

  while(sbush_cmd_tokens[position][0] != '\0' && position < COMMAND_MAX_ARGUMENTS){
    if(!strcmp(sbush_cmd_tokens[position], "&")){
      start_background = 1;
      sbush_cmd_tokens[position][0] = '\0';
    }
    position++;
  }

  //kprintf("Command: %s\n", sbush_cmd);
  if(!strcmp(sbush_cmd_tokens[0], "")) {
    return;
  } else if(!strcmp(sbush_cmd_tokens[0], "pwd")) {
    sbush_pwd_command(sbush_cmd_tokens);
  } else if(!strcmp(sbush_cmd_tokens[0], "cd")) {
    sbush_cd_command(sbush_cmd_tokens);
  } else if(!strcmp(sbush_cmd_tokens[0], "ls")) {
    sbush_ls_command(sbush_cmd_tokens);
  } else if(!strcmp(sbush_cmd_tokens[0], "echo")) {
    sbush_echo_command(sbush_cmd_tokens);
  } else if(!strcmp(sbush_cmd_tokens[0], "ps")) {
    sbush_ps_command(sbush_cmd_tokens);
  } else if(!strcmp(sbush_cmd_tokens[0], "cat")) {
    sbush_cat_command(sbush_cmd_tokens);
  }/* else if(!strcmp(sbush_cmd_tokens[0], "clear")) {
    sbush_clear_command(sbush_cmd_tokens);
  }*/ /*else if(!strcmp(sbush_cmd_tokens[0], "sleep")) {
    sbush_sleep_command(sbush_cmd_tokens);
  }*/ else if(!strcmp(sbush_cmd_tokens[0], "export")) {
    sbush_export_command(sbush_cmd_tokens);
  } else if(!strcmp(sbush_cmd_tokens[0], "kill")) {
    sbush_kill_command(sbush_cmd_tokens);
  } else if(!strcmp(sbush_cmd_tokens[0], "shutdown")) {
    sbush_shutdown_command(sbush_cmd_tokens);
  } else if(!strcmp(sbush_cmd_tokens[0], "user")) {
    //p_switch_to_user_mode();
    int ans = printf("Testing syscall from printf.\n");
    ans = printf("returned value = %d.\n", ans);
    printf("returned value = %d.\n", return_value);
    ans = printf("s");
    printf("returned value = %d.\n", ans);
    ans = printf("");
    printf("returned value = %d.\n", ans);
    /*int val;
    __asm__ __volatile__ (
    "int $0x80;"
    : "=a" (val)
    : "0"(SYSCALL_KPRINTF)
    : "cc", "rcx", "r11", "memory"
    );*/
  } else {
    kprintf("%s: command not found\n", sbush_cmd_tokens[0]);
    pid = fork();
    if(pid == 0){
      //const char *file, char *const argv[]
      /*return_code = execvp(sbush_cmd_tokens[0], sbush_cmd_tokens);
      if(return_code == -1){
        fprintf(stderr, "sbush:error:no such command:'%s'\n", sbush_cmd_tokens[0]);
      }*/
    } else if(pid < 0) {
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

//  kprintf("Inside test sbush\n");

  /*e_init_environment();
  kprintf("Initialized environment.\n");*/

  while(!feof(stdin)){
    char *sbush_cmd = NULL;
    char current_working_directory[64];

//    if(argc == 2){
//      sbush_execute_script(argv);
//    }

    /*pcb *temp = p_get_current_process();
    kprintf("pid = %d\n", temp->pid);*/

    if(getcwd(current_working_directory, sizeof(current_working_directory)) == NULL) {
      current_working_directory[0] = '\0';
    }
    kprintf("%s%s%s", "sbush:", current_working_directory, "$ ");

    /*char *ch = (char *) malloc(sizeof(char) * 2);
    ch[0] = 'j';
    ch[1] = '\0';
    kprintf("%s\n", ch);*/
    sbush_cmd = sbush_get_cmd();
    sbush_execute_cmd(sbush_cmd);

  }
  return 0;
}