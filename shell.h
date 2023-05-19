#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <limits.h>

/* --- Built-in Errors --- */
#define BUFSIZE 256
#define ENOSTRING 1106
#define EILLEGAL 227
#define EWSIZE 410
#define ENOBUILTIN 415
#define EBADCD 726

extern char **environ;

/**
 * struct linkedList - linked list data structure
 * @string: environ variable path name
 * @next: pointer to next node
 */
typedef struct linkedList
{
    char *string;
    struct linkedList *next;
} linked_l;

/**
 * struct configurations - configuration of build settings
 * @env: linked list of local env variables
 * @env_list: array of env variables to put into execve
 * @args: array of argument strings
 * @buffer: string buffer of user input
 * @path: array of $PATH locations
 * @full_path: string of path with correct prepended $PATH
 * @shell_name: name of shell (argv[0])
 * @count_line: counter of lines users have entered
 * @error_status: error status of last child process
 */
typedef struct configurations
{
    linked_l *env;
    char **env_list;
    char **args;
    char *buffer;
    char *path;
    char *full_path;
    char *shell_name;
    unsigned int count_line;
    int error_status;
} config;

/**
 * struct builtInCommands - commands and functions associated with it
 * @command: input command
 * @func: output function
 */
typedef struct builtInCommands
{
    char *command;
    int (*func)(config *build);
} type_b;

/* --- main --- */
config *config_init(config *build);

/* --- shell --- */
void shell(config *build);
int validate_input(config *build);
void remove_comments(char *str);
void fork_and_execute(config *build);
void convert_llist_to_arr(config *build);

/* shell_controls */
void insert_null_bytes(char *str, unsigned int index);
void display_prompt(void);
void add_new_line(void);
void signal_int(int sigint);

/* --- built_ins --- */
_Bool find_built_ins(config *build);
int execute_exit(config *build);
int execute_history(config *build);
int execute_alias(config *build);

/* --- built_in_controls --- */
int count_args(char **args);
int _atoi(char *s);

/* --- change_dir--- */
int execute_cd(config *build);
_Bool cd_to_home(config *build);
_Bool cd_to_previous(config *build);
_Bool cd_to_custom(config *build);
_Bool update_environ(config *build);

/* --- change_dir2 --- */
int update_old(config *build);
_Bool update_cur_dir(config *build, int index);

/* --- env_variables --- */
int env_function(config *build);
int set_env_func(config *build);
int unset_env_func(config *build);
int is_valid_variable(char *var);
int is_alpha(int c);
char *create_buffer(char *key, char *value);

/* --- help_funs --- */
int help_function(config *build);
int display_help_menu(void);
int help_guide_exit(config *build);
int help_env(config *build);
int help_history(config *build);

/* --- help_funs2 --- */
int help_alias(config *build);
int help_cd(config *biuld);
int help_set_env(config *build);
int help_unset_env(config *build);
int help_guide(config *build);

/* _getenv */
char *_getenv(char *input, char **environ);

/* handle_errors - managing wrong user inputs*/
void handle_errors(config *build);
char *get_error_message(void);
unsigned int count_num_digits(unsigned int num);
char *itoa(unsigned int num);

/* --- free --- */
void free_member(config *build);
void free_list(linked_l *head);
void free_args(char **args);
void free_args_and_buffer(config *build);

/* --- split_string --- */
_Bool split_string(config *build);
unsigned int count_words(char *str);
_Bool is_space(char c);

/* --- strings --- */
int _strlen(char *s);
char *_strcat(char *dest, char *src);
int _strcmp(char *s1, char *s2);
char *_strdup(char *str);
char *_strcpy(char *dest, char *src);

/* --- strings2 --- */
char *_strtok(char *str, char *delim);
int _strcspn(char *str, char *c);
char *_strchr(char *str, char ch);

/* --- path --- */
_Bool search_path(config *);
_Bool validate_constraints(config *build);
char *create_buffer(char *dir, char *cmd);

/* --- linked_list --- */
linked_l *add_node_to_front(linked_l **head, char *str);
linked_l *add_node_to_end(linked_l **head, char *str);
size_t print_list(const linked_l *h);
int delete_node_at_index(linked_l **head, unsigned int index);
size_t list_len(linked_l *h);

/* --- linkedlist_funs2 --- */
int find_node(linked_l *head, char *str);
linked_l *generateLinkedList(char **array);
linked_l *add_node_at_index(linked_l **head, int index, char *str);
char *get_node_at_index(linked_l *head, unsigned int index);

/* --- _realloc --- */
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
char *_memcpy(char *dest, const char *src, unsigned int n);

#endif
