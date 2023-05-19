#include "shell.h"
#include <stdlib.h>

/**
* main - entry point
* @argc: argument count
* @argv: argument vector
* Return: 0 on success
*/

int main(int argc, char **argv)
{
	config build;
	(void)argc;
	signal(SIGINT, signal_int);
	config_init(&build);
	build.shell_name = argv[0];
	shell(&build);
	return (0);
}
/**
* config_init - initialize member values for config struct
* @build: input build
* Return: build with initialized members
*/

config *config_init(config *build)
{
	build->env = generateLinkedList(environ);
	build->env_list = NULL;
	build->args = NULL;
	build->buffer = NULL;
	build->path = getenv("PATH");
	build->full_path = NULL;
	build->count_line = 0;
	build->shell_name = NULL;
	build->error_status = 0;
	return (build);
}
