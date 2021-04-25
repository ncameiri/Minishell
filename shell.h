/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/18 23:59:14 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/24 19:57:28 by tisantos         ###   ########.fr       */
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

typedef struct s_splvariab
{
	char			**tab;
	char			*next_str;
	unsigned int	next_str_len;
	unsigned int	nb_strs;
	unsigned int	i;
	int single_q;
	int double_q;

}			t_splvariab;

typedef struct s_linklis
{
	char			*pre_split;
	char			**content;
	int				type;
	struct s_linklis	*next;
}				t_linklis;

typedef struct s_minishell
{
	// LIST
	t_linklis 		*ls_start;
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
int			process_cmd_tables();

/*	History */

void			save_history();

/* List */
int 			add_to_list(int index);

/*	Utils */

void			free_global(char *f1, char *f2, char *f3, char *f4);
int				*add_int_to_arr(int *array, int location, int count);
char			**add_str_to_arrarr(char **array, char *string);
char			**ft_aloc_env(char **env);
char			**ft_split_igquo(char const *s, const char *delimiters);


#endif
