/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_tables2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/24 17:20:10 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/25 17:54:54 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

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
