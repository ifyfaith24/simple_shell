#include "shell.h"

/**
 * builtin_env - prints the current environment
 * @config: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int builtin_env(config_t *config)
{
	print_list_str(config->env);
	return (0);
}

/**
 * builtin_setenv - Initialize a new environment variable,
 *             or modify an existing one
 * @config: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int builtin_setenv(config_t *config)
{
	if (config->argc != 3)
	{
		err_puts("Incorrect number of arguements\n");
		return (1);
	}
	if (_setenv(config, config->argv[1], config->argv[2]))
		return (0);
	return (1);
}

/**
 * builtin_unsetenv - Remove an environment variable
 * @config: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int builtin_unsetenv(config_t *config)
{
	int i;

	if (config->argc == 1)
	{
		err_puts("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= config->argc; i++)
		_unsetenv(config, config->argv[i]);

	return (0);
}

/**
 * fill_env_list - populates env linked list
 * @config: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int fill_env_list(config_t *config)
{
	s_array *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		add_node_end(&node, environ[i], 0);
	config->env = node;
	return (0);
}

/**
 * _setenv - Initialize a new environment variable,
 *             or modify an existing one
 * @config: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * @value: the string env var value
 *  Return: Always 0
 */
int _setenv(config_t *config, char *var, char *value)
{
	char *buf = NULL;
	s_array *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(_strlen(var) + _strlen(value) + 2);
	if (!buf)
		return (1);
	_strcpy(buf, var);
	_strcat(buf, "=");
	_strcat(buf, value);
	node = config->env;
	while (node)
	{
		p = starts_with(node->val, var);
		if (p && *p == '=')
		{
			free(node->val);
			node->val = buf;
			config->env_flag = 1;
			return (0);
		}
		node = node->next;
	}
	add_node_end(&(config->env), buf, 0);
	free(buf);
	config->env_flag = 1;
	return (0);
}
