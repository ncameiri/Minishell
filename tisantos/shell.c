/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/18 23:57:53 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/20 16:08:14 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char **shell_split_arguments(char *line)
{
	char **args;

	args = NULL;
	args = ft_split_chars(line, SHELL_DELIMITERS);

	return (args);
}

char *shell_prompt(void)
{
	char *line;
	char buf[4096 + 1];
	char *path;

	line = NULL;
	path = getcwd(buf, 4096);

	write(1, "\x1b[33m", 6);
	write(1, path, ft_strlen(path));
	write(1, "\x1b[0m", 5);
	write(1, "$ ", 3);

	get_next_line(0, &line);

	return (line);
}

void shell_loop(char **env)
{
	char *line;
	char **args;
	int status;

	do
	{
		line = shell_prompt();
		args = shell_split_arguments(line);
		//status = shell_execute(args);

		//int execvp(const char *__file, char *const *__argv)
		//int execve(const char *__path, char **__argv, char **__envp)


		execve(args[0], args, env);

		free(line);
		free_array(args);
	} while (status);
}



int main (int argc, char **argv, char **env)
{

	shell_loop(env);

	return (EXIT_SUCCESS);
}
