/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/18 23:59:14 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/28 11:37:15 by tisantos         ###   ########.fr       */
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


typedef struct s_splvariab
{
	char			**tab;
	char			*next_str;
	unsigned int	next_str_len;
	unsigned int	nb_strs;
	unsigned int	i;
	int 			single_q;
	int 			double_q;

}				t_splvariab;

typedef struct s_linklis
{
	char				*pre_split;
	char				**content;
	int					type;
	int					builtin;
	struct s_linklis	*next;
}				t_linklis;

typedef struct	s_var_seplink
{
	int		i;
	int		s;
	int		single_q;
	int		double_q;
	char	n;
}				t_var_seplink;
typedef struct s_var_add_tlis
{
	int		i;
	t_list	*temp;
	char	*aux;
	int		start;
	int		last;
	int		type;
}t_var_add_tlis;
typedef struct s_minishell
{
	// List

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

	// PID

	pid_t			pid;


	// Add to List
	int 			error;

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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/18 23:59:14 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/28 16:38:45 by tisantos         ###   ########.fr       */
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


typedef struct s_splvariab
{
	char			**tab;
	char			*next_str;
	unsigned int	next_str_len;
	unsigned int	nb_strs;
	unsigned int	i;
	int 			single_q;
	int 			double_q;

}				t_splvariab;

typedef struct s_linklis
{
	char				*pre_split;
	char				**content;
	int					type;
	struct s_linklis	*next;
}				t_linklis;

typedef struct	s_var_seplink
{
	int		i;
	int		s;
	int		single_q;
	int		double_q;
	char	n;
}				t_var_seplink;
typedef struct s_var_add_tlis
{
	int		i;
	t_list	*temp;
	char	*aux;
	int		start;
	int		last;
	int		type;
}t_var_add_tlis;
typedef struct s_minishell
{
	// List

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

	// PID

	pid_t			pid;

	// Add to List
	int 			error;

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

void			exec_command();

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

int				*add_int_to_arr(int *array, int location, int count);
char			**add_str_to_arrarr(char **array, char *string);
int				only_spaces(char *line);
void			debug(int a);

/*	Signals */

void			sig_int(int signo);
void			sig_quit(int signo);

/*	List */

int				add_to_list(int index);
void			ft_lstspli(void);
void			ft_lsttrim(void);
void			ft_linklstclear(t_linklis **lst);
int				is_separator(char check, char check2, int *type);
void			add_var_init(t_var_add_tlis *va);
void 			chck_dup_symbols(void);
void 			chck_begend_symbols(void);
char			**ft_aloc_env(char **env);
char			**ft_split_igquo(char const *s, const char *delimiters);
int 			chck_iespac(char s1,char s);

/*	Free */

void			free_global(char *f1, char *f2, char *f3, char *f4);
void			exit_cntrl_d();

#endif
