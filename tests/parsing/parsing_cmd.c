/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_tiago.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/27 18:36:31 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/28 14:21:48 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../get_next_line/get_next_line.h"
# include "../../libft/libft.h"

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <string.h>

# define SHELL_DELIMITERS " \t\r\n\a"

# define STDIN 0
# define STDOUT 1
# define STDERR 2

/**************************************************************/
/*						Structs Stuff						  */
/**************************************************************/

typedef struct s_splvariab
{
	char			**tab;
	char			*next_str;
	unsigned int	next_str_len;
	unsigned int	nb_strs;
	unsigned int	i;
	int 			single_q;
	int 			double_q;

}						t_splvariab;
typedef struct s_linklis
{
	char				*pre_split;
	char				**content;
	int					type;
	struct s_linklis	*next;
}						t_linklis;
typedef struct	s_var_seplink
{
	int		i;
	int		s;
	int		single_q;
	int		double_q;
	char	n;
}						t_var_seplink;
typedef struct s_var_add_tlis
{
		int		i;
	t_list	*temp;
	char	*aux;
	int		start;
	int		last;
	int		type;
}						t_var_add_tlis;
typedef struct s_minishell
{
	t_linklis 		*ls_start;
	char			*line; // Line you write on your stdin.
	char			**cmd_tables; // Command tables where you store your commands separated by ;
	char			**args; // Line divided in arguments.
	char			**env; // Env file.
	int				status; // Status of while loop.
	int				absolute_path; // 1 to change to absolute, 0 for minishell 	>.
	int				testing; // Changes subject commands for real builtin commands.
	char			**history; // Saves the history of commands you typed.
	int				history_len; // How many commands you've typed.
	pid_t			pid;
	int 			error;
}					t_minishell;

t_minishell mini_sh;



/**************************************************************/
/*							Parsing							  */
/**************************************************************/

/* Utils */

int	only_spaces(char *line)
{
	int i;
	int a;

	i = 0;
	a = 0;
	while (line[i] != '\0')
	{
		if (line[i] != ' ')
			a++;
		i++;
	}
	if (a > 0)
		return (0);
	else
		return (1);
}

int	*add_int_to_arr(int *array, int location, int count)
{
	int a;
	int i;
	int *temp;

	i = 0;
	a = 0;

	while (i < count)
		i++;
	temp = malloc(sizeof(int) * (i + 2));
	if (temp == NULL)
		return (NULL);
	while(a < i)
	{
		temp[a] = array[a];
		a++;
	}
	temp[a++] = location;
	temp[a] = '\0';

	free(array);

	return (temp);
}

char	**add_str_to_arrarr(char **array, char *string)
{
	int a;
	int i;
	char **temp;

	i = 0;
	a = 0;
	while (array[i] != NULL)
		i++;
	temp = malloc(sizeof(char *) * (i + 2));
	if (temp == NULL)
		return (NULL);
	while(a < i)
	{
		temp[a] = ft_strdup(array[a]);
		a++;
	}
	temp[a++] = ft_strdup(string);
	temp[a] = NULL;
	free_array(array);
	return (temp);
}

void	free_global(char *f1, char *f2, char *f3, char *f4)
{
	if (!ft_strcmp(f1, "line") || !ft_strcmp(f2, "line")
		|| !ft_strcmp(f3, "line") || !ft_strcmp(f4, "line"))
	{
		free(mini_sh.line);
		mini_sh.line = NULL;
	}
	if (!ft_strcmp(f1, "history") || !ft_strcmp(f2, "history")
		|| !ft_strcmp(f3, "history") || !ft_strcmp(f4, "history"))
	{
		free_array(mini_sh.history);
		mini_sh.history = NULL;
	}
	if (!ft_strcmp(f1, "cmd_tables") || !ft_strcmp(f2, "cmd_tables")
		|| !ft_strcmp(f3, "cmd_tables") || !ft_strcmp(f4, "cmd_tables"))
	{
		free_array(mini_sh.cmd_tables);
		mini_sh.cmd_tables = NULL;
	}
	if (!ft_strcmp(f1, "env") || !ft_strcmp(f2, "env")
		|| !ft_strcmp(f3, "env") || !ft_strcmp(f4, "env"))
	{
		free_array(mini_sh.env);
		mini_sh.env = NULL;
	}
	if (!ft_strcmp(f1, "args") || !ft_strcmp(f2, "args")
		|| !ft_strcmp(f3, "args") || !ft_strcmp(f4, "args"))
	{
		free_array(mini_sh.args);
		mini_sh.args = NULL;
	}
}

char **shell_split_args(char *line) // <-- Temporária só.
{
	char **args;

	args = NULL;
	args = ft_split_chars(line, SHELL_DELIMITERS);

	return (args);
}


/* Command Table 3 */

void	syntax_error(int value)
{
	if (value == 1)
		printf("%s\n", "bash: syntax error near unexpected token `;'");
	else if(value == 2)
		printf("%s\n", "bash: syntax error near unexpected token `;;'");
}

void	remove_cmd_blanks2()
{
	int i;
	int a;
	int c;
	int copy;
	char *temp;

	i = 0;
	a = 0;
	c = 0;
	copy = 0;
	while (mini_sh.cmd_tables[i] != NULL)
	{
		copy = 0;
		if (mini_sh.cmd_tables[i][0] == ' ')
		{
			a = 0;
			while (mini_sh.cmd_tables[i][a] == ' ' && mini_sh.cmd_tables[i][a] != '\0')
			{
				a++;
				copy++;
			}
			temp = ft_strdup(mini_sh.cmd_tables[i] + copy);
			free(mini_sh.cmd_tables[i]);
			mini_sh.cmd_tables[i] = ft_strdup(temp);
			free(temp);
		}
		i++;
	}
}

void	remove_cmd_blanks()
{
	int i;
	int a;
	int not_null;

	i = 0;
	a = 0;
	not_null = 0;
	while (mini_sh.cmd_tables[i] != NULL)
		i++;
	i -= 1;
	while (mini_sh.cmd_tables[i][a] != '\0')
		if (mini_sh.cmd_tables[i][a++] != ' ')
			not_null++;
	if (not_null == 0)
	{
		free(mini_sh.cmd_tables[i]);
		mini_sh.cmd_tables[i] = NULL;
	}
	i = 0;
	remove_cmd_blanks2();
}

void	remove_cmd_semicolons()
{
	int i;

	i = 0;
	while (mini_sh.cmd_tables[i] != NULL)
	{
		if (mini_sh.cmd_tables[i][ft_strlen(mini_sh.cmd_tables[i]) - 1] == ';')
			mini_sh.cmd_tables[i][ft_strlen(mini_sh.cmd_tables[i]) - 1] = '\0';
		i++;
	}
	i = 0;
}

int	final_cmd_error_handling(int i, int a, int c)
{
	while (mini_sh.cmd_tables[i] != NULL)
	{
		a = 0;
		c = 0;
		if (mini_sh.cmd_tables[i][0] == ';')
		{
			syntax_error(1);
			return (0);
		}
		while (mini_sh.cmd_tables[i][a] != '\0')
		{
			if (mini_sh.cmd_tables[i][a] != ' ' &&
				mini_sh.cmd_tables[i][a] != ';')
				c++;
			if (mini_sh.cmd_tables[i][a] == ';' &&
				c == 0)
			{
				syntax_error(1);
				return (0);
			}
			a++;
		}
		i++;
	}
	return (1);
}

int	initial_cmd_error_handling(int *semicolon_location, int semicolon_count)
{
	int i;

	i = 0;
	if (semicolon_location[0] == 0)
	{
		if (semicolon_count == 1)
			syntax_error(1);
		else if(semicolon_count > 1)
		{
			if (semicolon_location[1] == 1)
				syntax_error(2);
			else
				syntax_error(1);
		}
		return 0;
	}
	while (i < semicolon_count)
	{
		if ((semicolon_location[i + 1] - semicolon_location[i]) == 1)
		{
			syntax_error(2);
			return (0);
		}
		i++;
	}
	return (1);
}


/* Command Table 2 */

int	split_cmd_tables3(int *semicolon_location, int i, int copy, int last)
{
	int a;

	a = 0;

	mini_sh.cmd_tables[i] = malloc(sizeof(char) * (ft_strlen(mini_sh.line) + 2));
	if (mini_sh.cmd_tables[i] == NULL)
		return (0);
	if (last == 0)
	{
		while (copy <= semicolon_location[i])
			mini_sh.cmd_tables[i][a++] = mini_sh.line[copy++];
	}
	else
	{
		while (copy < ft_strlen(mini_sh.line))
			mini_sh.cmd_tables[i][a++] = mini_sh.line[copy++];
	}
	mini_sh.cmd_tables[i][a] = '\0';
	return (copy);
}

int	split_cmd_tables2(int *semicolon_location, int semicolon_count)
{
	int i;
	int copy;

	i = 0;
	copy = 0;

	if (initial_cmd_error_handling(semicolon_location, semicolon_count) == 0)
	{
		free(semicolon_location);
		return (0); // Se começa com ; ou se tem 2 ;; juntos.
	}

	mini_sh.cmd_tables = malloc(sizeof(char *) * (semicolon_count + 2));
	if (mini_sh.cmd_tables == NULL)
		return (0);
	while (i < semicolon_count)
		copy = split_cmd_tables3(semicolon_location, i++, copy, 0);
	if (copy < ft_strlen(mini_sh.line))
		copy = split_cmd_tables3(semicolon_location, i++, copy, 1);
	mini_sh.cmd_tables[i] = NULL;

	if (final_cmd_error_handling(0, 0, 0) == 0)
	{
		free_global("cmd_tables", "empty", "empty", "empty");
		free(semicolon_location);
		return (0);
	}
	remove_cmd_semicolons();
	remove_cmd_blanks();

	return (1);
}

int	split_cmd_tables(int *semicolon_location, int semicolon_count)
{
	int i;

	i = 0;

	if (semicolon_count == 0)
	{
		mini_sh.cmd_tables = malloc(sizeof(char *) * 2);
		mini_sh.cmd_tables[0] = ft_strdup(mini_sh.line);
		mini_sh.cmd_tables[1] = NULL;
		return (1);
	}
	else
	{
		if (split_cmd_tables2(semicolon_location, semicolon_count) == 0)
			return (0);
	}
	return (1);
}

int	*separator_location(int *semi_locations, int *semi_count)
{
	int i;
	int s;
	int single_q;
	int double_q;

	i = 0;
	s = 0;
	single_q = 0;
	double_q = 0;
	while (mini_sh.line[i] != '\0')
	{
		if (mini_sh.line[i] == '\"' && double_q == 0 && single_q == 0)
			double_q = 1;
		else if (mini_sh.line[i] == '\"' && double_q == 1 && single_q == 0)
			double_q = 0;
		else if (mini_sh.line[i] == '\'' && single_q == 0 && double_q == 0)
			single_q = 1;
		else if (mini_sh.line[i] == '\'' && single_q == 1 && double_q == 0)
			single_q = 0;
		if (mini_sh.line[i] == ';' && single_q == 0 && double_q == 0)
		{
			semi_locations = add_int_to_arr(semi_locations, i, s);
			s++;
		}
		i++;
	}
	*semi_count = s;
	return (semi_locations);
}

int	process_cmd_tables()
{
	int	*semicolon_location;
	int semicolon_count;

	semicolon_count = 0;

	semicolon_location = malloc(sizeof(int ) * 2);
	if (semicolon_location == NULL)
		return (0);
	semicolon_location[0] = '\0';

	semicolon_location = separator_location(semicolon_location, &semicolon_count);

	if (split_cmd_tables(semicolon_location, semicolon_count) == 0)
		return (0);

	free(semicolon_location);

	return (1);
}


/* Command Table */

int	check_complete_quotation()
{
	int i;
	int single_q;
	int double_q;

	i = 0;
	single_q = 0;
	double_q = 0;
	while (mini_sh.line[i] != '\0')
	{
		if (mini_sh.line[i] == '\"' && double_q == 0 && single_q == 0)
			double_q = 1;
		else if (mini_sh.line[i] == '\"' && double_q == 1 && single_q == 0)
			double_q = 0;
		else if (mini_sh.line[i] == '\'' && single_q == 0 && double_q == 0)
			single_q = 1;
		else if (mini_sh.line[i] == '\'' && single_q == 1 && double_q == 0)
			single_q = 0;
		i++;
	}
	if (single_q + double_q == 0)
		return (1);
	else
		return (0);
}

int	cmd_parsing()
{
	if (check_complete_quotation() == 0) // (Input is missing a " or '". It's invalid.).
	{
		printf("%s\n", "bash: quotation incomplete");
		free_global("line", "empty", "empty", "empty");
		return (0);
	}
	if (process_cmd_tables() == 0)
	{
		free_global("line", "empty", "empty", "empty");
		return (0);
	}
	return (1);
}



/**************************************************************/
/*							Main Stuff						  */
/**************************************************************/

int main ()
{
	int i = 0;

	while(i < 500)
	{
		mini_sh.line = NULL;
		get_next_line(0, &mini_sh.line);
		//**********************************************//

		if (cmd_parsing() == 0)
			continue;

		for (size_t a = 0; mini_sh.cmd_tables[a] != NULL; a++)
		{
			printf("%s\n", mini_sh.cmd_tables[a]);
		}


		//**********************************************//
		if (ft_strcmp(mini_sh.line, "exit") == 0)
		{
			free(mini_sh.line);
			free_array(mini_sh.cmd_tables);
			break;
		}
		free(mini_sh.line);
		free_array(mini_sh.cmd_tables);
	}
}
