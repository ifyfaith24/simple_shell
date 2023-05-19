#include "shell.h"
#include <stdlib.h>

/**
 * _strlen - returns the length of a string
 * @s: pointer to the string
 * Return: length of string
 */

int _strlen(char *s)
{
	int len = 0;

	while (*s != '\0')
	{
	len++;
	s++;
	}
	return (len);
}

/**
 * _strcat - function that concatenates two strings
 * @dest: copy destination
 * @src:  source of string
 * Return: concatenated string
 */

char *_strcat(char *dest, char *src)
{
	char *ptr = dest;

	while (*ptr != '\0')
	{
	ptr++;
	}
	while (*src != '\0')
	{
	*ptr = *src;
	ptr++;
	src++;
	}
	*ptr = '\0';
	return (dest);
}

/**
 * _strcmp - a function that compares two strings
 * @s1: string 1
 * @s2: string 2
 * Return: 1 if true, 0 if false
 */

int _strcmp(char *s1, char *s2)
{
	while (*s1 != '\0' && *s1 == *s2)
	{
	s1++;
	s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

/**
 * _strdup - returns a pointer to a newly allocated space in memory,
 * which contains a copy of the string given as a parameter.
 * @str: string to be copied
 * Return: copied string
 */

char *_strdup(char *str)
{
	if (str == NULL)
	{
	return (NULL);
	}
	int len = _strlen(str);
	char *copy = malloc(sizeof(char) * (len + 1));
	if (copy == NULL)
	{
	perror("Malloc failed\n");
	exit(errno);
	}
	char *ptr = copy;
	while (*str != '\0')
	{
	*ptr = *str;
	ptr++;
	str++;
	}
	*ptr = '\0';
	return (copy);
}

/**
 * *_strcpy - copies the string pointed to by src,
 * including the terminating null byte
 * @dest: copied string
 * @src: string to be copied
 * Return: pointer to new copied string
 */

char *_strcpy(char *dest, char *src)
{
	char *ptr = dest;
	while (*src != '\0')
	{
	*dest = *src;
	dest++;
	src++;
	}
	*dest = '\0';
	return (ptr);
}
