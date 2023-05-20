#include "shell.h"

/**
 *err_puts - prints an input string
 * @str: the string to be printed
 *
 * Return: Nothing
 */
void err_puts(char *str)
{
	int i;

    if (!str)
        return;
    for (i = 0; str[i] != '\0'; i++)
    {
        err_putchar(str[i]);
    }
}

/**
 * err_putchar - writes the character c to stderr
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int err_putchar(char c)
{
	static int i;
	static char buf[BUF_SIZE];

	if (c == FLUSH_BUF || i >= BUF_SIZE)
	{
		write(2, buf, i);
		i = 0;
	}
	if (c != FLUSH_BUF)
		buf[i++] = c;
	return (1);
}

/**
 * put_fd - writes the character c to given fd
 * @c: The character to print
 * @fd: The filedescriptor to write to
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int put_fd(char c, int fd)
{
	static int i;
	static char buf[BUF_SIZE];

	if (c == FLUSH_BUF || i >= BUF_SIZE)
	{
		write(fd, buf, i);
		i = 0;
	}
	if (c != FLUSH_BUF)
		buf[i++] = c;
	return (1);
}

/**
 *puts_fd - prints a string to a file descriptor
 * @str: the string to be printed
 * @fd: the file descriptor to write to
 *
 * Return: the number of characters printed
 */
int puts_fd(char *str, int fd)
{
	int i = 0;

	if (!str)
		return (0);

	for (int j = 0; str[j] != '\0'; j++)
	{
		i += put_fd(str[j], fd);
	}

	return (i);
}

/**
 * print_error - prints an error message
 * @config: the parameter & return config struct
 * @estr: string containing specified error type
 * Return: 0 if no numbers in string, converted number otherwise
 *        -1 on error
 */
void print_error(config_t *config, char *estr)
{
	err_puts(config->file_name);
	err_puts(": ");
	print_d(config->error_count, STDERR_FILENO);
	err_puts(": ");
	err_puts(config->argv[0]);
	err_puts(": ");
	err_puts(estr);
}
