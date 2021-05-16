/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/18 23:57:53 by tisantos          #+#    #+#             */
/*   Updated: 2021/05/16 03:15:49 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	loop_command_tables()
{
	int i;

	i = 0;
	while (mini_sh.cmd_tables[i] != NULL)
	{
		if (add_to_list(i) == 0)
			break;
		//debug_command_table(i); // Debugger. Podemos apagar no fim.
		add_to_simple_commands_list();
		//debug_simple_commands();
		exec_command();
		ft_linklstclear(&mini_sh.ls_start);
		ft_lstclear_simple_struct(&mini_sh.simple_cmd);
		i++;
	}
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

void	shell_prompt()
{
	char buf[BUFSIZ];
	int i;
	int nb_char_read;

	i = 0;

	ft_bzero(buf, BUFSIZ);
	mini_sh.current_history = mini_sh.history_len;

	mini_sh.line = NULL;

	shell_prompt_line();

	while(!ft_strchr(buf, '\n'))
	{
		nb_char_read = read(STDIN_FILENO, &buf[i], BUFSIZ - i);

		if (*buf == 4)
		{
			turn_on_canonical_mode();
			write (1, "exit\n", 6);
			exit_finale(0);
			exit(0);
		}
		else if (buf[i] == 3)
		{
			printf("^C\n");
			shell_prompt_line();
			ft_bzero(buf, BUFSIZ);
			i = 0;
		}
		else if (buf[i] == 28 || buf[i] == 4 || buf[i] == 26)
		{
			if (buf != '\0');
				buf[i-1];
		}
		else if (is_up_down_arrow(&buf[i]))
			parse_input_history(buf, &i);
		else if (ft_strcmp(&buf[i], mini_sh.backspace) == 0)
			delete_single_char(buf, &i);
		else if (nb_char_read > 1)
			ft_bzero(&buf[i], BUFSIZ - i);
		else
			i += write(STDOUT_FILENO, &buf[i], 1);
	}
	buf[i - 1] = '\0';
	mini_sh.line = ft_strdup(buf);
	if (only_spaces(mini_sh.line) == 1)
		free_global("line", "empty", "empty", "empty");
	if (mini_sh.line == NULL || mini_sh.line[0] == '\0')
		shell_prompt();
}

void	shell_loop()
{
	while (mini_sh.status == 1)
	{
		turn_off_canonical_mode();
		shell_prompt();
		save_history();
		turn_on_canonical_mode();
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
	mini_sh.islinux = 1;
	mini_sh.dollar_error = errno;

	signal(SIGQUIT, &sig_quit);
	signal(SIGINT, &sig_int);

	init_termcaps();

	//shell_loop();

	return (0);
}
