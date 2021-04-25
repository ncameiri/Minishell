/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/25 14:15:02 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/25 18:05:38 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

void	sig_int(int signo)
{
	char *path;
	char buf[1001];

	path = getcwd(buf, 1000);

	if (mini_sh.pid != 0)
	{
		kill(mini_sh.pid, SIGINT);
		printf("\n");
		mini_sh.pid = 0;
	}
	else if (mini_sh.pid == 0)
	{
		if (mini_sh.absolute_path == 0)
		{
			printf("\n");
			write(1, "\x1b[90mminishell\x1b[91m$ ", 22);
		}
		else
		{
			printf("\n");
			write(1, "\x1b[90m", 6);
			write(1, path, ft_strlen(path));
			write(1, "\x1b[91m$ ", 8);
		}
	}
}

void	sig_quit(int signo)
{
	char backspace;
	char empty;
	char *sig_number;

	backspace = 8;
	empty = ' ';

	sig_number = ft_itoa(signo);

	if (mini_sh.pid != 0)
	{
		kill(mini_sh.pid, SIGQUIT);
		ft_putstr_fd("Quit (core dumped)", STDERR);
		write (1, "\n", 1);
		mini_sh.pid = 0;
	}
	else
	{
		write(2, &backspace, 1);
		write(2, &backspace, 1);
		write(2, &empty, 1);
		write(2, &empty, 1);
		write(2, &backspace, 1);
		write(2, &backspace, 1);
	}
	free(sig_number);
}

