/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/18 23:59:14 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/23 02:27:54 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include "./get_next_line/get_next_line.h"
# include "./libft/libft.h"

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <string.h>

# define LSH_RL_BUFSIZE 1024
# define LSH_TOK_BUFSIZE 64
# define SHELL_DELIMITERS " \t\r\n\a"

typedef struct s_minishell
{
	// Freeable

	char			**args; // Line divided in arguments.
	char			*line; // Line you write on your stdin.

	// Not Freeable

	char			**env; // Env file.
	int				status; // Status of while loop.

	// Added Builtins

	int				absolute_path; // 1 to change to absolute, 0 for minishell 	>.
	int				testing; // Changes subject commands for real builtin commands.

}					t_minishell;

t_minishell mini_sh;

void			exec_func();

/* 	Builtins */

void			ft_absolute_path();
void			ft_testing();
int				ft_pwd();
int				ft_echo();
void 			ft_env();
int				ft_cd();

#endif
