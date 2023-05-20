#include "shell.h"

/**
 * get_environ - returns the string array copy of our environ
 * @config: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **get_environ(config_t *config)
{
	if (!config->environ || config->env_flag)
	{
		config->environ = list_to_strings(config->env);
		config->env_flag = 0;
	}

	return (config->environ);
}

/**
 * _unsetenv - Remove an environment variable
 * @config: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: 1 on delete, 0 otherwise
 * @var: the string env var property
 */
int _unsetenv(config_t *config, char *var)
{
	s_array *node = config->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = starts_with(node->val, var);
		if (p && *p == '=')
		{
			config->env_flag = delete_node_at_index(&(config->env), i);
			i = 0;
			node = config->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (config->env_flag);
}

/**
 * get_env_value - gets the value of an environ variable
 * @config: Structure containing potential arguments. Used to maintain
 * @name: env var name
 *
 * Return: the value
 */
char *get_env_value(config_t *config, const char *name)
{
	s_array *node = config->env;
	char *p;

	while (node)
	{
		p = starts_with(node->val, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * copy_path  - copies characters from one buffer to another
 * @src: the source buffer
 * @start: starting index
 * @stop: stopping index
 *
 * Return: pointer to new buffer
 */
char *copy_path(char *pathsrc, int start, int stop)
{
	static char buf[1024];
	int i = start, k = 0;

	while (i < stop && pathsrc[i] != ':')
	{
		buf[k++] = pathsrc[i++];
	}
	buf[k] = '\0';

	return (buf);
}

/**
 * get_cmd_path - finds this cmd in the PATH string
 * @config: the config struct
 * @pathsrc: the PATH string
 * @cmd: the cmd to find
 *
 * Return: full path of cmd if found or NULL
 */
char *get_cmd_path(config_t *config, char *pathsrc, char *cmd)
{
	int i = 0, curr_pos = 0;
	char *path;

	if (!pathsrc)
		return (NULL);
	if ((_strlen(cmd) > 2) && starts_with(cmd, "./"))
	{
		if (is_exec_cmd(config, cmd))
			return (cmd);
	}
	while (1)
	{
		if (!pathsrc[i] || pathsrc[i] == ':')
		{
			path = copy_path(pathsrc, curr_pos, i);
			if (!*path)
				_strcat(path, cmd);
			else
			{
				_strcat(path, "/");
				_strcat(path, cmd);
			}
			if (is_exec_cmd(config, path))
				return (path);
			if (!pathsrc[i])
				break;
			curr_pos = i;
		}
		i++;
	}
	return (NULL);
}
