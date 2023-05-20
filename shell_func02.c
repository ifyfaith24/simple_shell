#include "shell.h"

/**
 * is_interactive - checks if shell is in interactive mode
 * @config: struct address
 *
 * Return: 1 if in interactive mode, 0 otherwise
 */
int is_interactive(config_t *config)
{
	return (isatty(STDIN_FILENO) && config->input_fd <= 2);
}

/**
 * strip_comments - function replaces first instance of '#' with '\0'
 * @buf: address of the string to modify
 *
 * Return: Always 0;
 */
void strip_comments(char *buf)
{
	int i;

	for (i = 0; buf[i] != '\0'; i++)
		if (buf[i] == '#' && (!i || buf[i - 1] == ' '))
		{
			buf[i] = '\0';
			break;
		}
}

/**
 * buf_chained_cmds - buffers chained commands from user input
 * @config: configuration struct
 * @buf: pointer to buffer
 * @len: pointer to length variable
 *
 * Return: number of bytes read
 */
ssize_t buf_chained_cmds(config_t *config, char **buf, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len) /* if nothing left in the buffer, fill it */
	{
		/*free_ptr((void **)config->hsh_buf);*/
		free(*buf);
		*buf = NULL;
		signal(SIGINT, handle_sigint);
		r = read_line(config, buf, &len_p);
		if (r > 0)
		{
			if ((*buf)[r - 1] == '\n')
			{
				(*buf)[r - 1] = '\0'; /* remove trailing newline */
				r--;
			}
			config->error_flag = 1;
			strip_comments(*buf);
			add_history_entry(config, *buf, config->hist_count++);
			/* if (_strchr(*buf, ';')) is this a command chain? */
			{
				*len = r;
				config->hsh_buf = buf;
			}
		}
	}
	return (r);
}

/**
 * get_cmd_input - gets user input and processes command chains
 * @config: parameter struct
 *
 * Return: length of input read, or -1 on EOF
 */
ssize_t get_cmd_input(config_t *config)
{
	static char *buf; /* the ';' command chain buffer */
	static size_t i, j, len;
	ssize_t r = 0;
	char **buf_p = &(config->arg), *p;

	_putchar(FLUSH_BUF);
	r = buf_chained_cmds(config, &buf, &len);
	if (r == -1) /* EOF */
		return (-1);
	if (len)	/* we have commands left in the chain buffer */
	{
		j = i; /* init new iterator to current buf position */
		p = buf + i; /* get pointer for return */

		update_chain_pos(config, buf, &j, i, len);
		while (j < len) /* iterate to semicolon or end */
		{
			if (is_chain_delim(config, buf, &j))
				break;
			j++;
		}

		i = j + 1; /* increment past nulled ';'' */
		if (i >= len) /* reached end of buffer? */
		{
			i = len = 0; /* reset position and length */
			config->hsh_buf_type = CMD_INIT;
		}

		*buf_p = p; /* pass back pointer to current command position */
		return (_strlen(p)); /* return length of current command */
	}

	*buf_p = buf; /* else not a chain, pass back buffer from read_line () */
	return (r); /* return length of buffer from read_line () */
}

/**
 * read_input_buf - reads a buffer from the input file descriptor
 * @config: a pointer to a configuration struct
 * @buf: a pointer to the buffer to be read into
 * @i: a pointer to the size of the buffer
 *
 * Return: the number of bytes read or -1 if an error occurred
 */
ssize_t read_input_buf(config_t *config, char *buf, size_t *i)
{
	ssize_t r = 0;

	if (*i)
		return (0);
	r = read(config->input_fd, buf, BUF_SIZE);
	if (r >= 0)
		*i = r;
	return (r);
}
