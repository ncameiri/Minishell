/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 18:09:40 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/25 18:49:16 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

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
