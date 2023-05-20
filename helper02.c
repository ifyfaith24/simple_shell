#include "shell.h"

/**
 * update_chain_pos - checks we should continue chaining based on last status
 * @config: pointer to the configuration struct
 * @buf: the char buffer
 * @p: address of current position in buf
 * @i: starting position in buf
 * @len: length of buf
 *
 * Return: Void
 */
void update_chain_pos(config_t *config, char *buf, size_t *p, size_t i, size_t len)
{
	size_t j = *p;

	if (config->hsh_buf_type == AND_CMD)
	{
		if (config->status)
		{
			buf[i] = 0;
			j = len;
		}
	}
	if (config->hsh_buf_type == OR_CMD)
	{
		if (!config->status)
		{
			buf[i] = 0;
			j = len;
		}
	}

	*p = j;
}

/**
 * change_vars - replaces vars in the tokenized string
 * @config: pointer to the configuration struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int change_vars(config_t *config)
{
	int i = 0;
	s_array *node;

	for (i = 0; config->argv[i]; i++)
	{
		if (config->argv[i][0] != '$' || !config->argv[i][1])
			continue;

		if (!_strcmp(config->argv[i], "$?"))
		{
			update_string(&(config->argv[i]),
				_strdup(num_to_string(config->status, 10, 0)));
			continue;
		}
		if (!_strcmp(config->argv[i], "$$"))
		{
			update_string(&(config->argv[i]),
				_strdup(num_to_string(getpid(), 10, 0)));
			continue;
		}
		node = node_starts_with(config->env, &config->argv[i][1], '=');
		if (node)
		{
			update_string(&(config->argv[i]),
				_strdup(_strchr(node->val, '=') + 1));
			continue;
		}
		update_string(&config->argv[i], _strdup(""));

	}
	return (0);
}

/**
 * is_delim - checks if character is a delimeter
 * @c: the char to check
 * @delim: the delimeter string
 * Return: 1 if true, 0 if false
 */
int is_delim(char c, char *delim)
{
    for (int i = 0; delim[i] != '\0'; i++)
    {
        if (c == delim[i])
            return 1;
    }
    return 0;
}

/**
 * is_chain_delim -  test if current char in buffer is a chain delimiter
 * @config: pointer to the configuration struct
 * @buf: the char buffer
 * @p: address of current position in buf
 *
 * Return: 1 if chain delimeter, 0 otherwise
 */
int is_chain_delim(config_t *config, char *buf, size_t *p)
{
	size_t j = *p;

	if (buf[j] == '|' && buf[j + 1] == '|')
	{
		buf[j] = 0;
		j++;
		config->hsh_buf_type = OR_CMD;
	}
	else if (buf[j] == '&' && buf[j + 1] == '&')
	{
		buf[j] = 0;
		j++;
		config->hsh_buf_type = AND_CMD;
	}
	else if (buf[j] == ';') /* found end of this command */
	{
		buf[j] = 0; /* replace semicolon with null */
		config->hsh_buf_type = CHAIN_CMD;
	}
	else
		return (0);
	*p = j;
	return (1);
}

/**
 * free_ptr - frees a pointer and NULLs the address
 * @ptr: address of the pointer to free
 *
 * Return: 1 if freed, otherwise 0.
 */
int free_ptr(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
		return (1);
	}
	return (0);
}
