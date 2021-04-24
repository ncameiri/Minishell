/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/18 23:57:53 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/24 01:39:50 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*void sighandler(int num)//Adicionei esta
{
	printf(" %d clicaste Ctrl + C\n",num);
	exit(1);
}*/


void	*shell_prompt()
{
	char *line;
	char buf[1001];
	char *path;

	line = NULL;
	path = getcwd(buf, 1000);

	if (mini_sh.absolute_path == 0)
	{
		write(1, "\x1b[90m", 6);
		write(1, "minishell", 10);
		write(1, "\x1b[91m", 6);
		write(1, "$ ", 3);
	}
	else
	{
		write(1, "\x1b[90m", 6);
		write(1, path, ft_strlen(path));
		write(1, "\x1b[91m", 6);
		write(1, "$ ", 3);
	}

	get_next_line(0, &mini_sh.line);
}

char **shell_split_args(char *line) // <-- Temporária só.
{
	char **args;

	args = NULL;
	args = ft_split_chars(line, SHELL_DELIMITERS);

	return (args);
}

void	shell_loop()
{
	mini_sh.status = 1;

	//signal(SIGINT, sighandler);//para reconhecer o ctrl +c

	while (mini_sh.status == 1)
	{
		shell_prompt();
		if (mini_sh.line == NULL || mini_sh.line[0] == '\0')
			continue;
		save_history(); // guardar os lines todos para o mini_sh.history seta cima e baixo.
		//if (parsing() == 0)
		//	continue;

		mini_sh.args = shell_split_args(mini_sh.line); // <-- Temporária só.

		exec_func();

		free(mini_sh.line);
		//free_array(mini_sh.cmd_tables);
	}
}



int	main (int argc, char **argv, char **env)
{
	(void)argc; // This is for -Wall -Wextra -Werror not
	(void)argv; // to give unused variables error.

	mini_sh.absolute_path = 0; // Set absolute path as prompt message or Minishell as Prompt.
	mini_sh.testing = 0; // Changes subject commands for real builtin commands for test purposes.
	mini_sh.env = env; // <-- Meter este alocado, uma função qualquer, get_env(), qualquer coisa.

	shell_loop();

	return (0);
}
