#include "shell.h"

/**
 * insert_null_bytes - insert null bytes at a given index
 * @str: input string
 * @index: contains  the null bytes
 * Return: Nothing
 */

void insert_null_bytes(char *str, unsigned int index)
{
	str[index] = '\0';
}

/**
 * display_prompt - accepts users inputs
 * Return: Nothing
 */

void display_prompt(void)
{
	write(STDERR_FILENO, "$ ", 2);
}

/**
 * add_new_line - prints a newline
 * Return: Nothing
 */

void add_new_line(void)
{
	write(STDOUT_FILENO, "\n", 1);
}

/**
 * signal_int - a function to handle signal integers
 * @sigint: signal input
 * Return: Nothing
 */

void signal_int(int sigint)
{
	(void)sigint;
	signal(SIGINT, signal_int);
	add_new_line();
	add_new_line();
	fflush(stdout);
}
