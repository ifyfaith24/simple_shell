#include "shell.h"

/**
 * shell - a function that executes the
 * loop's functionalities
 * @build: input build params
 *
 * Description: reviews user inputs and validates strings;
 * otherwise, check the path, fork and execute it
 * Return: Nothing
 */

void shell(config *build)
{
	while (1)
	{
	validate_input(build);
    	if
	(validate_input(build) == 0)
	{
		continue;
	}
    	if
	(find_built_ins(build) == 1)
	{
		continue;
	}
	search_path(build);
	fork_and_execute(build);
	}
}

/**
 * validate_input- a function to handle the user inputs
 * @build: input build params
 * Description: gets user inputs, checks input against
 * build constraints, string edge cases and takes endless inputs
 * Return: Nothing
*/

int validate_input(config *build)
{
	size_t buffer_size = 0;
	ssize_t input_length;
	char *newline_ptr, *tab_ptr;

	build->args = NULL;
	build->env_list = NULL;
	build->count_line++;

	if (isatty(STDIN_FILENO))
	{
		getopt;
	}

	input_length = getline(&build->buffer, &buffer_size, stdin);

	if (input_length == EOF)
	{
	if (isatty(STDIN_FILENO))
	{
		add_new_line();
	}
	if (build->error_status)
	{
		exit(build->error_status);
	}
	exit(EXIT_SUCCESS);
	}

	newline_ptr = strchr(build->buffer, '\n');
	tab_ptr = strchr(build->buffer, '\t');

	if (newline_ptr || tab_ptr != NULL)
	{
		insert_null_bytes(build->buffer, input_length - 1);
	}

	remove_comments(build->buffer);
}

/**
 * remove_comments - remove comments from input string
 * @str: input string
 * Return: length of remaining string
 */

void remove_comments(char *str)
{
	int index = 0;
	_Bool is_not_first = false;

	while (str[index])
	{
	if (index == 0 && str[index] == '#')
	{
	insert_null_bytes(str, index);
	return;
	}
	if (is_not_first)
	{
		if (str[index] == '#' && str[index - 1] == ' ')
		{
			insert_null_bytes(str, index);
			return;
		}
	}
	index++;
	is_not_first = true;
	}
}

void fork_and_execute(config *build)
{
	int status;
	pid_t child_pid = fork();

	convert_llist_to_arr(build);

	if (child_pid == -1)
	{
	perror("error\n");
	free_member(build);
	free_args(build->env_list);
	exit(1);
	}

	if (child_pid == 0)
	{
	if (execve(build->full_path, build->args, build->env_list) == -1)
	{
		handle_errors(build);
		free_member(build);
		free_args(build->env_list);
		if (errno == ENOENT)
		exit(127);
		if (errno == EACCES)
		exit(126);
	}
	}
	else
	{
	wait(&status);
	if (WIFEXITED(status))
	{
		build->error_status = WEXITSTATUS(status);
	}
	free_args_and_buffer(build);
	free_args(build->env_list);
	}
}

/**
 * convert_llist_to_arr - convert linked list to array
 * @build: input build params
 */

void convert_linked_list_to_array(config *build)
{
	int index = 0;
	size_t count = 0;
	char **env_list = NULL;
	linked_l *temp = build->env;

	count = list_len(build->env);
	env_list = malloc(sizeof(char *) * (count + 1));

	if (!env_list)
	{
	perror("Malloc failed\n");
	exit(1);
	}

	while (temp)
	{
	env_list[index] = strdup(temp->string);
	temp = temp->next;
	index++;
	}

	env_list[index] = NULL;
	build->env_list = env_list;
	free(env_list);
}
