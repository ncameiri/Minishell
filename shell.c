/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/18 23:57:53 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/24 19:53:43 by tisantos         ###   ########.fr       */
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
	int i;
	//signal(SIGINT, sighandler);//para reconhecer o ctrl +c

	while (mini_sh.status == 1)
	{
		i = 0;
		shell_prompt();
		if (mini_sh.line == NULL || mini_sh.line[0] == '\0')
			continue;
		save_history(); // guardar os lines todos para o mini_sh.history seta cima e baixo.
		if (cmd_parsing() == 0)
			continue;

		
		while (mini_sh.cmd_tables[i] != NULL)
		{
			//mini_sh.args = shell_split_args(mini_sh.cmd_tables[i]);
			add_to_list(i);
			mini_sh.args = NULL;
			mini_sh.args = shell_split_args(mini_sh.cmd_tables[i]); // <-- Temporária só.
			//exec_func();
			
			//exec_func();
			i++;
		}

		free(mini_sh.line);
		//free_array(mini_sh.args);
		free_array(mini_sh.cmd_tables);
		//mini_sh.status = 0;
	}
	free_array(mini_sh.history);
	free_array(mini_sh.env);
}



int	main (int argc, char **argv, char **env)
{
	(void)argc; // This is for -Wall -Wextra -Werror not
	(void)argv; // to give unused variables error.

	mini_sh.absolute_path = 0; // Set absolute path as prompt message or Minishell as Prompt.
	mini_sh.testing = 0; // Changes subject commands for real builtin commands for test purposes.
	mini_sh.env = ft_aloc_env(env); // <-- Meter este alocado, uma função qualquer, get_env(), qualquer coisa. tá no utils
	shell_loop();

	return (0);
}
