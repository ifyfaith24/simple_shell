#include "shell.h"

/**
 * builtin_exit - exits the shell
 * @config: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: exits with a given exit status
 *         (0) if config.argv[0] != "exit"
 */
int builtin_exit(config_t *config)
{
	int exitcheck;

	if (config->argv[1])  /* If there is an exit arguement */
	{
		exitcheck = safe_atoi(config->argv[1]);
		if (exitcheck == -1)
		{
			config->status = 2;
			print_error(config, "Illegal number: ");
			err_puts(config->argv[1]);
			err_putchar('\n');
			return (1);
		}
		config->err_code = safe_atoi(config->argv[1]);
		return (-2);
	}
	config->err_code = -1;
	return (-2);
}

/**
 * builtin_cd - changes the current directory of the process
 * @config: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int builtin_cd(config_t *config)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!config->argv[1])
	{
		dir = get_env_value(config, "HOME=");
		if (!dir)
			chdir_ret = /* TODO: what should this be? */
				chdir((dir = get_env_value(config, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (_strcmp(config->argv[1], "-") == 0)
	{
		if (!get_env_value(config, "OLDPWD="))
		{
			_puts(s);
			_putchar('\n');
			return (1);
		}
		_puts(get_env_value(config, "OLDPWD=")), _putchar('\n');
		chdir_ret = /* TODO: what should this be? */
			chdir((dir = get_env_value(config, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(config->argv[1]);
	if (chdir_ret == -1)
	{
		print_error(config, "can't cd to ");
		err_puts(config->argv[1]), err_putchar('\n');
	}
	else
	{
		_setenv(config, "OLDPWD", get_env_value(config, "PWD="));
		_setenv(config, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * builtin_help - changes the current directory of the process
 * @config: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int builtin_help(config_t *config)
{
	char **arg_array;

	arg_array = config->argv;
	_puts("help call works. Function not yet implemented \n");
	if (0)
		_puts(*arg_array); /* temp att_unused workaround */
	return (0);
}

/**
 * builtin_history - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int builtin_history(config_t *info)
{
	print_list(info->history);
	return (0);
}
