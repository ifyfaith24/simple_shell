#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

#define BUF_SIZE 1024
#define FLUSH_BUF -1
#define CMD_INIT	0
#define OR_CMD		1
#define AND_CMD		2
#define CHAIN_CMD	3
#define C_LOWER	1
#define C_UNSIGNED	2
#define HISTORY_FILE	".history"
#define MAX_HISTORY	4096

extern char **environ;

/**
 * struct arraylst - singly linked list
 * @key: the number field
 * @val: a string
 * @next: points to the next node
 */
typedef struct arraylst
{
	int key;
	char *val;
	struct arraylst *next;
} s_array;

/**
 * struct config - contains pseudo-arguments to pass into a function,
 * allowing a uniform prototype for function pointer struct
 * @arg: a string generated from getline containing arguments
 * @argv: an array of strings generated from arg
 * @path: a string path for the current command
 * @argc: the argument count
 * @error_count: the error count
 * @err_code: the error code for exit()s
 * @error_flag: if on count this line of input
 * @file_name: the program filename
 * @environ: custom modified copy of environ
 * @env: linked list local copy of environ
 * @history: the history node
 * @alias: the alias node
 * @env_flag: on if environ was changed
 * @status: the return status of the last exec'd command
 * @hsh_buf: address of pointer to hsh_buf, on if chaining
 * @hsh_buf_type: Command type ||, &&, ;
 * @input_fd: the fd from which to read line input
 * @hist_count: the history line number count
 */

typedef struct config {
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int error_count;
	int err_code;
	int error_flag;
	char *file_name;
	char **environ;
	s_array *env;
	int env_flag;
	int status;
	s_array *history;
	s_array *alias;
	char **hsh_buf;
	int hsh_buf_type;
	int input_fd;
	int hist_count;
} config_t;

/* Initialization value for a `config_t` struct */
#define CONFIG_INIT { \
	NULL,     /* arg */ \
	NULL,     /* argv */ \
	NULL,     /* path */ \
	0,        /* argc */ \
	0,        /* error_count */ \
	0,        /* err_code */ \
	0,        /* error_flag */ \
	NULL,     /* file_name */ \
	NULL,     /* environ */ \
	NULL,     /* env */ \
	0,        /* env_flag */ \
	0,        /* status */ \
	NULL,     /* history */ \
	NULL,     /* alias */ \
	NULL,     /* hsh_buf */ \
	0,        /* hsh_buf_type */ \
	0,        /* input_fd */ \
	0         /* hist_count */ \
}

/**
 * struct builtincmd - contains information about
 * built-in command and its corresponding function
 * @command: the name of the built-in command
 * @handler: the function to handle the built-in command
 */
typedef struct builtincmd
{
	char *command;
	int (*handler)(config_t *);
} builtincmd_t;


int run_shell(config_t *, char **);

int is_builtin(config_t *);
void locate_cmd(config_t *);
void fork_cmd(config_t *);

/* toem_parser.c */
int is_exec_cmd(config_t *, char *);
char *copy_path(char *, int, int);
char *get_cmd_path(config_t *, char *, char *);

/* toem_errors.c */
void err_puts(char *);
int err_putchar(char);
int put_fd(char c, int fd);
int puts_fd(char *str, int fd);

/* toem_string.c */
int _strlen(char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *_strcat(char *, char *);

/* toem_string1.c */
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

/* toem_exits.c */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

/* toem_tokenizer.c */
char **strtow(char *, char *);
char **strtow2(char *, char);

/* toem_realloc.c */
char *_memset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);

/* toem_memory.c */
int free_ptr(void **);

/* toem_atoi.c */
int is_interactive(config_t *);
int is_delim(char, char *);
int _isalpha(int);
int _atoi(char *);

/* toem_errors1.c */
int safe_atoi(char *);
void print_error(config_t *, char *);
int print_d(int, int);
char *num_to_string(long int, int, int);
void strip_comments(char *);

/* toem_builtin.c */
int builtin_exit(config_t *);
int builtin_cd(config_t *);
int builtin_help(config_t *);

/* toem_builtin1.c */
int builtin_history(config_t *);
int builtin_alias(config_t *);

/*toemread_line .c */
ssize_t get_cmd_input(config_t *);
int read_line(config_t *config, char **ptr, size_t *length);
void handle_sigint(int);

/* toem_getinfo.c */
void clear_config(config_t *);
void set_config(config_t *, char **);
void free_config(config_t *, int);

/* toem_environ.c */
char *get_env_value(config_t *, const char *);
int builtin_env(config_t *);
int builtin_setenv(config_t *);
int builtin_unsetenv(config_t *);
int fill_env_list(config_t *);

/* toemget_env_value.c */
char **get_environ(config_t *);
int _unsetenv(config_t *, char *);
int _setenv(config_t *, char *, char *);

/* toem_history.c */
char *fetch_history_file(config_t *config);
int save_history(config_t *config);
int load_history(config_t *config);
int add_history_entry(config_t *config, char *buf, int linecount);
int update_hist_nums(config_t *config);

/* toem_lists.c */
s_array *add_node(s_array **, const char *, int);
s_array *add_node_end(s_array **, const char *, int);
size_t print_list_str(const s_array *);
int delete_node_at_index(s_array **, unsigned int);
void free_list(s_array **);

/* toem_lists1.c */
size_t list_len(const s_array *);
char **list_to_strings(s_array *);
size_t print_list(const s_array *);
s_array *node_starts_with(s_array *, char *, char);
ssize_t get_node_index(s_array *, s_array *);

int is_chain_delim(config_t *, char *, size_t *);
void update_chain_pos(config_t *, char *, size_t *, size_t, size_t);
int change_alias(config_t *);
int change_vars(config_t *);
int update_string(char **, char *);
ssize_t read_input_buf(config_t *config, char *buf, size_t *len);


#endif
