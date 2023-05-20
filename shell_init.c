#include "shell.h"

/**
 * run_shell - main shell loop
 * @config: the parameter & return config struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int run_shell(config_t *config, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		clear_config(config);
		if (is_interactive(config))
			_puts("$ ");
		err_putchar(FLUSH_BUF);
		r = get_cmd_input(config);
		if (r != -1)
		{
			set_config(config, av);
			builtin_ret = is_builtin(config);
			if (builtin_ret == -1)
				locate_cmd(config);
		}
		else if (is_interactive(config))
			_putchar('\n');
		free_config(config, 0);
	}
	save_history(config);
	free_config(config, 1);
	if (!is_interactive(config) && config->status)
		exit(config->status);
	if (builtin_ret == -2)
	{
		if (config->err_code == -1)
			exit(config->status);
		exit(config->err_code);
	}
	return (builtin_ret);
}

/**
 * is_builtin - checks if a command is a built-in command
 * @config: the parameter to the config list
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int is_builtin(config_t *config)
{
	int i, builtin_ret = -1;
	builtincmd_t builtin_cmds[] = {
		{"exit", builtin_exit},
		{"env", builtin_env},
		{"help", builtin_help},
		{"history", builtin_history},
		{"setenv", builtin_setenv},
		{"unsetenv", builtin_unsetenv},
		{"cd", builtin_cd},
		{"alias", builtin_alias},
		{NULL, NULL}
	};

	for (i = 0; builtin_cmds[i].command; i++)
		if (_strcmp(config->argv[0], builtin_cmds[i].command) == 0)
		{
			config->error_count++;
			builtin_ret = builtin_cmds[i].handler(config);
			break;
		}
	return (builtin_ret);
}

/**
 * locate_cmd - finds a command in PATH
 * @config: the parameter & return config struct
 *
 * Return: void
 */
void locate_cmd(config_t *config)
{
	char *path = NULL;
	int i, k;

	config->path = config->argv[0];
	if (config->error_flag == 1)
	{
		config->error_count++;
		config->error_flag = 0;
	}
	for (i = 0, k = 0; config->arg[i]; i++)
		if (!is_delim(config->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = get_cmd_path(config, get_env_value(config, "PATH="), config->argv[0]);
	if (path)
	{
		config->path = path;
		fork_cmd(config);
	}
	else
	{
		if ((is_interactive(config) || get_env_value(config, "PATH=")
			|| config->argv[0][0] == '/') && is_exec_cmd(config, config->argv[0]))
			fork_cmd(config);
		else if (*(config->arg) != '\n')
		{
			config->status = 127;
			print_error(config, "not found\n");
		}
	}
}

/**
 * fork_cmd - forks a process to run a command
 * and waits for it to complete
 * @config: pointer to configuration struct
 *
 * Return: void
 */
void fork_cmd(config_t *config)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(config->path, config->argv, get_environ(config)) == -1)
		{
			free_config(config, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
	}
	else
	{
		wait(&(config->status));
		if (WIFEXITED(config->status))
		{
			config->status = WEXITSTATUS(config->status);
			if (config->status == 126)
				print_error(config, "Permission denied\n");
		}
	}
}

/**
 * is_exec_cmd - determines if a file is an executable command
 * @config: the config struct
 * @path: path to the file
 *
 * Return: 1 if true, 0 otherwise
 */
int is_exec_cmd(config_t *config, char *path)
{
	struct stat st;

	(void)config;
	if (!path || stat(path, &st))
		return (0);

	if (st.st_mode & S_IFREG)
	{
		return (1);
	}
	return (0);
}
