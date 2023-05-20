#include "shell.h"

/**
 * main - entry point
 * @ac: arg count
 * @av: arg vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int argc, char **argv)
{
	config_t config[] = { CONFIG_INIT };
	int fd = 2;

	asm ("mov %1, %0\n\t"
		"add $3, %0"
		: "=r" (fd)
		: "r" (fd));

	if (argc == 2)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				err_puts(argv[0]);
				err_puts(": 0: Can't open ");
				err_puts(argv[1]);
				err_putchar('\n');
				err_putchar(FLUSH_BUF);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		config->input_fd = fd;
	}
	fill_env_list(config);
	load_history(config);
	run_shell(config, argv);
	return (EXIT_SUCCESS);
}
