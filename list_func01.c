#include "shell.h"

/**
 * add_node - adds a node to the start of the list
 * @head: address of pointer to head node
 * @str: str field of node
 * @num: node index used by history
 *
 * Return: size of list
 */
s_array *add_node(s_array **head, const char *str, int num)
{
	s_array *new_head;

	if (!head)
		return (NULL);
	new_head = malloc(sizeof(s_array));
	if (!new_head)
		return (NULL);
	_memset((void *)new_head, 0, sizeof(s_array));
	new_head->key = num;
	if (str)
	{
		new_head->val = _strdup(str);
		if (!new_head->val)
		{
			free(new_head);
			return (NULL);
		}
	}
	new_head->next = *head;
	*head = new_head;
	return (new_head);
}

/**
 * add_node_end - adds a node to the end of the list
 * @head: address of pointer to head node
 * @str: str field of node
 * @num: node index used by history
 *
 * Return: size of list
 */
s_array *add_node_end(s_array **head, const char *str, int num)
{
	s_array *new_node, *node;

	if (!head)
		return (NULL);

	node = *head;
	new_node = malloc(sizeof(s_array));
	if (!new_node)
		return (NULL);
	_memset((void *)new_node, 0, sizeof(s_array));
	new_node->key = num;
	if (str)
	{
		new_node->val = _strdup(str);
		if (!new_node->val)
		{
			free(new_node);
			return (NULL);
		}
	}
	if (node)
	{
		while (node->next)
			node = node->next;
		node->next = new_node;
	}
	else
		*head = new_node;
	return (new_node);
}

/**
 * print_list_str - prints only the str element of a s_array linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t print_list_str(const s_array *h)
{
	size_t i = 0;

	while (h)
	{
		_puts(h->val ? h->val : "(nil)");
		_puts("\n");
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * delete_node_at_index - deletes node at given index
 * @head: address of pointer to first node
 * @index: index of node to delete
 *
 * Return: 1 on success, 0 on failure
 */
int delete_node_at_index(s_array **head, unsigned int index)
{
	s_array *node, *prev_node;
	unsigned int i = 0;

	if (!head || !*head)
		return (0);

	if (!index)
	{
		node = *head;
		*head = (*head)->next;
		free(node->val);
		free(node);
		return (1);
	}
	node = *head;
	while (node)
	{
		if (i == index)
		{
			prev_node->next = node->next;
			free(node->val);
			free(node);
			return (1);
		}
		i++;
		prev_node = node;
		node = node->next;
	}
	return (0);
}

/**
 * free_list - frees all nodes of a list
 * @head_ptr: address of pointer to head node
 *
 * Return: void
 */
void free_list(s_array **head_ptr)
{
	s_array *node, *next_node, *head;

	if (!head_ptr || !*head_ptr)
		return;
	head = *head_ptr;
	node = head;
	while (node)
	{
		next_node = node->next;
		free(node->val);
		free(node);
		node = next_node;
	}
	*head_ptr = NULL;
}
