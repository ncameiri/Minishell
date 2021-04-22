/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/18 23:57:53 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/22 03:42:26 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void sighandler(int num)//Adicionei esta
{
	printf(" %d clicaste Ctrl + C\n",num);
	exit(1);
}

char **shell_split_args(char *line)
{
	char **args;

	args = NULL;
	args = ft_split_chars(line, SHELL_DELIMITERS);

	return (args);
}

char *shell_prompt()
{
	char *line;
	char buf[1001];
	char *path;

	line = NULL;
	path = getcwd(buf, 1000);

	if (mini_sh.absolute_path == 0)
	{
		write(1, "\x1b[94m", 6);
		write(1, "minishell", 10);
		write(1, "\x1b[93m", 6);
		write(1, "$ ", 3);
	}
	else
	{
		write(1, "\x1b[94m", 6);
		write(1, path, ft_strlen(path));
		write(1, "\x1b[93m", 6);
		write(1, "$ ", 3);
	}

	get_next_line(0, &line);

	return (line);
}

void shell_loop()
{
	mini_sh.status = 1;
	signal(SIGINT, sighandler);//para reconhecer o ctrl +c
	while (mini_sh.status == 1)
	{
		mini_sh.line = shell_prompt();
		if (mini_sh.line == NULL)
			continue;

		mini_sh.args = shell_split_args(mini_sh.line);

		exec_func();

		free(mini_sh.line);
		free_array(mini_sh.args);
	}
}



int main (int argc, char **argv, char **env)
{
	(void)argc; // This is for -Wall -Wextra -Werror not
	(void)argv; // to give unused variables error.
	mini_sh.absolute_path = 0; // Set absolute path as prompt message or Minishell as Prompt.
	mini_sh.testing = 0; // Changes subject commands for real builtin commands for test purposes.
	mini_sh.env = env;
	shell_loop();

	return (0);
}
