/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/18 23:59:14 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/25 18:16:56 by tisantos         ###   ########.fr       */
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

# define SHELL_DELIMITERS " \t\r\n\a"

# define STDIN 0
# define STDOUT 1
# define STDERR 2

typedef struct s_minishell
{
	// Parsed

	char			*line; // Line you write on your stdin.

	char			**cmd_tables; // Command tables where you store your commands separated by ;
	char			**args; // Line divided in arguments.

	// Env

	char			**env; // Env file.

	// Loop

	int				status; // Status of while loop.

	// Added Builtins

	int				absolute_path; // 1 to change to absolute, 0 for minishell 	>.
	int				testing; // Changes subject commands for real builtin commands.

	// History

	char			**history; // Saves the history of commands you typed.
	int				history_len; // How many commands you've typed.

	// PID

	pid_t			pid;

}					t_minishell;

/* Freeable
**
**		**args
**		*line
**		**history
**		**cmd_tables
**		**env
**
*/

t_minishell mini_sh;


/*	Command Tables */

void			syntax_error(int value);
int				initial_cmd_error_handling(int *semicolon_location, int semicolon_count);
int				final_cmd_error_handling();
void			remove_cmd_semicolons();
void			remove_cmd_blanks();

/*	Exec */

void			exec_func();

/* 	Builtins */

void			ft_absolute_path();
void			ft_testing();
void			ft_history();
int				ft_pwd();
int				ft_echo();
void 			ft_env();
int				ft_cd();

/* 	Parsing */

int				cmd_parsing();
int				process_cmd_tables();

/*	Allocate */

void			save_history();
char			**save_env(char **env);

/*	Utils */

void			free_global(char *f1, char *f2, char *f3, char *f4);
int				*add_int_to_arr(int *array, int location, int count);
char			**add_str_to_arrarr(char **array, char *string);
int				only_spaces(char *line);
char			**shell_split_args(char *line);

/*	Signals */

void			sig_int(int signo);
void			sig_quit(int signo);


#endif
