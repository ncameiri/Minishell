/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/18 23:57:53 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/25 20:30:14 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	loop_command_tables()
{
	int i;
	int a;

	i = 0;
	a = 0;
	while (mini_sh.cmd_tables[i] != NULL)
	{
		if (a++ > 0)
			free_array(mini_sh.args);
		add_to_list(i);
		ft_lstspli(); //<--- Importante
		ft_lsttrim();
		mini_sh.args = shell_split_args(mini_sh.cmd_tables[i++]); // <-- Temporária só.
		exec_func();
	}
	if (a > 0)
		free_global("args", "cmd_tables", "line", "empty");
	else
		free_global("cmd_tables", "line", "empty", "empty");
}

void	*shell_prompt_line()
{
	char buf[1001];
	char *path;

	path = getcwd(buf, 1000);

	if (mini_sh.absolute_path == 0)
		write(1, "\x1b[90mminishell\x1b[91m$ ", 22);
	else
	{
		write(1, "\x1b[90m", 6);
		write(1, path, ft_strlen(path));
		write(1, "\x1b[91m$ ", 8);
	}
}

void	*shell_prompt()
{
	char *line;

	line = NULL;
	mini_sh.line = NULL;

	shell_prompt_line();
	if (get_next_line(0, &mini_sh.line) == 0)
	{
		write (1, "exit\n", 6);
		exit(0);
	}
	if (only_spaces(mini_sh.line) == 1)
		free_global("line", "empty", "empty", "empty");
	if (mini_sh.line == NULL || mini_sh.line[0] == '\0')
		shell_prompt();
}

void	shell_loop()
{
	while (mini_sh.status == 1)
	{
		shell_prompt();
		save_history();
		if (cmd_parsing() == 0)
			continue;
		loop_command_tables();
	}
}

int	main (int argc, char **argv, char **env)
{
	(void)argc; // This is for -Wall -Wextra -Werror not
	(void)argv; // to give unused variables error.

	mini_sh.absolute_path = 0; // Set absolute path as prompt message or Minishell as Prompt.
	mini_sh.testing = 0; // Changes subject commands for real builtin commands for test purposes.
	mini_sh.env = save_env(env); // <-- Meter este alocado, uma função qualquer, get_env(), qualquer coisa. tá no utils
	mini_sh.pid = 0;	// Até agora  é para matar o filho no signals.
	mini_sh.status = 1;	// Para o loop o shell_loop não parar.

	signal(SIGINT, &sig_int);	// cntrl-c
	signal(SIGQUIT, &sig_quit);	// cntrl-\

	shell_loop();

	return (0);
}
