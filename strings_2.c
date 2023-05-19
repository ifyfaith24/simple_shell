#include "shell.h"

/**
 * _strtok - tokenizes strings at delimiter
 * @str: input string
 * @delim: delimiter
 * Return: pointer to start of string
 */

char *_strtok(char *str, char *delim)
{
	static char *ptr;
	char *start;
	char *i;
	char current;

	if (str == NULL)
	{
	str = ptr;
	}

	while (*str && _strchr(delim, *str))
	{
	str++;
	}

	if (*str == '\0')
	{
	return (NULL);
	}

	start = str;

	while (*str && !_strchr(delim, *str))
	{
	str++;
	}

	if (*str != '\0')
	{
	*str = '\0';
	ptr = str + 1;
	}
	else
	{
	ptr = str;
	}

	return (i);
}

/**
 * _strcspn - returns first occurence of any char in
 * second string in first string
 * @string: input string to search
 * @chars: input chars to check
 * Return: pointer to first match
 */

int _strcspn(char *str, char *c)
{
	int count = 0;
	char current;

	while (*str)
	{
	current = *str;
	if (_strchr(c, current))
	{
		break;
	}
	str++;
	count++;
	}

	return (count);
}

/**
 * _strchr - locates a character in a string
 * @str: string to be searched
 * @ch: target char
 * Return: pointer to first occurrence of c or
 * NULL if char not found
 */

char *_strchr(char *str, char ch)
{
	while (*str)
	{
	if (*str == ch)
	{
	return (str);
	}
	str++;
	}

	return (NULL);
}
