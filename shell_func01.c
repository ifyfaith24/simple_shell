#include "shell.h"

/**
 * clear_config - initializes config_t struct
 * @config: struct address
 */
void clear_config(config_t *config)
{
	config->arg = NULL;
	config->argv = NULL;
	config->path = NULL;
	config->argc = 0;
}

/**
 * set_config - initializes config_t struct
 * @config: struct address
 * @av: argument vector
 */
void set_config(config_t *config, char **av)
{
	int i = 0;

	config->file_name = av[0];
	if (config->arg)
	{
		config->argv = strtow(config->arg, " \t");
		if (!config->argv)
		{

			config->argv = malloc(sizeof(char *) * 2);
			if (config->argv)
			{
				config->argv[0] = _strdup(config->arg);
				config->argv[1] = NULL;
			}
		}
		for (i = 0; config->argv && config->argv[i]; i++)
			;
		config->argc = i;

		change_alias(config);
		change_vars(config);
	}
}

/**
 * free_config - frees info_t struct fields
 * @config: struct address
 * @all: true if freeing all fields
 */
void free_config(config_t *config, int all)
{
	ffree(config->argv);
	config->argv = NULL;
	config->path = NULL;
	if (all)
	{
		if (!config->hsh_buf)
			free(config->arg);
		if (config->env)
			free_list(&(config->env));
		if (config->history)
			free_list(&(config->history));
		if (config->alias)
			free_list(&(config->alias));
		ffree(config->environ);
			config->environ = NULL;
		free_ptr((void **)config->hsh_buf);
		if (config->input_fd > 2)
			close(config->input_fd);
		_putchar(FLUSH_BUF);
	}
}

/**
 * read_line - reads the next line of input from STDIN
 * @config: pointer to the configuration struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: the number of characters read, or -1 on failure
 */
int read_line(config_t *config, char **ptr, size_t *length)
{
	static char buf[BUF_SIZE];
	static size_t i, len;
	size_t k;
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		s = *length;
	if (i == len)
		i = len = 0;

	r = read_input_buf(config, buf, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = _strchr(buf + i, '\n');
	k = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = _realloc(p, s, s ? s + k : k + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);

	if (s)
		_strncat(new_p, buf + i, k - i);
	else
		_strncpy(new_p, buf + i, k - i + 1);

	s += k - i;
	i = k;
	p = new_p;

	if (length)
		*length = s;
	*ptr = p;
	return (s);
}

/**
 * handle_sigint - blocks ctrl-C
 * @sig_num: the signal number
 *
 * Return: void
 */
void handle_sigint(__attribute__((unused))int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(FLUSH_BUF);
}
