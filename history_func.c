#include "shell.h"

/**
 * fetch_history_file - gets the history file
 * @config: parameter struct
 *
 * Return: allocated string containg history file
 */

char *fetch_history_file(config_t *config)
{
	char *buf, *dir;

	dir = get_env_value(config, "HOME=");
	if (!dir)
		return (NULL);
	buf = malloc(sizeof(char) * (_strlen(dir) + _strlen(HISTORY_FILE) + 2));
	if (!buf)
		return (NULL);
	buf[0] = 0;
	_strcpy(buf, dir);
	_strcat(buf, "/");
	_strcat(buf, HISTORY_FILE);
	return (buf);
}

/**
 * save_history - creates a file, or appends to an existing file
 * @config: pointer to the configuration struct
 *
 * Return: 1 on success, else -1
 */
int save_history(config_t *config)
{
	ssize_t fd;
	char *filename = fetch_history_file(config);
	s_array *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = config->history; node; node = node->next)
	{
		puts_fd(node->val, fd);
		put_fd('\n', fd);
	}
	put_fd(FLUSH_BUF, fd);
	close(fd);
	return (1);
}

/**
 * load_history - reads history from file
 * @config: pointer to the configuration struct
 *
 * Return: hist_count on success, 0 otherwise
 */
int load_history(config_t *config)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buf = NULL, *filename = fetch_history_file(config);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buf = malloc(sizeof(char) * (fsize + 1));
	if (!buf)
		return (0);
	rdlen = read(fd, buf, fsize);
	buf[fsize] = 0;
	if (rdlen <= 0)
		return (free(buf), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0;
			add_history_entry(config, buf + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		add_history_entry(config, buf + last, linecount++);
	free(buf);
	config->hist_count = linecount;
	while (config->hist_count-- >= MAX_HISTORY)
		delete_node_at_index(&(config->history), 0);
	update_hist_nums(config);
	return (config->hist_count);
}

/**
 * add_history_entry - adds a new entry to the command history linked list
 * @config: configuration struct
 * @buf: buffer containing the command string
 * @linecount: the line number in the history
 *
 * Return: Always 0
 */
int add_history_entry(config_t *config, char *buf, int linecount)
{
	s_array *node = NULL;

	if (config->history)
		node = config->history;
	add_node_end(&node, buf, linecount);

	if (!config->history)
		config->history = node;
	return (0);
}

/**
 * update_hist_nums - updates the line numbers of the command history entries
 * @config: the configuration struct containing the command history
 *
 * Return: the new history count
 */
int update_hist_nums(config_t *config)
{
	s_array *node = config->history;
	int new_count = 0;

	while (node)
	{
	node->key = new_count++;
	node = node->next;
	}

	config->hist_count = new_count;
	return (new_count);
}
