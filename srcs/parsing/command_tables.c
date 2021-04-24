/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_tables.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/24 00:36:21 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/24 19:50:15 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

int	split_cmd_tables3(int *semicolon_location, int i, int copy, int last)
{
	int a;

	a = 0;

	mini_sh.cmd_tables[i] = malloc(sizeof(char) * (ft_strlen(mini_sh.line) + 2));
	if (mini_sh.cmd_tables[i] == NULL)
		return 0;
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

void	split_cmd_tables2(int *semicolon_location, int semicolon_count)
{
	int i;
	int copy;

	i = 0;
	copy = 0;

	if (initial_cmd_error_handling(semicolon_location, semicolon_count) == 0)
		return ; // Se começa com ; ou se tem 2 ;; juntos.

	mini_sh.cmd_tables = malloc(sizeof(char *) * (semicolon_count + 2));
	if (mini_sh.cmd_tables == NULL)
		return ;
	while (i < semicolon_count)
		copy = split_cmd_tables3(semicolon_location, i++, copy, 0);
	if (copy < ft_strlen(mini_sh.line))
		copy = split_cmd_tables3(semicolon_location, i++, copy, 1);
	mini_sh.cmd_tables[i] = NULL;

	if (final_cmd_error_handling(0, 0, 0) == 0)
		return ;
	remove_cmd_semicolons();
	remove_cmd_blanks();
}

void	split_cmd_tables(int *semicolon_location, int semicolon_count)
{
	int i;

	i = 0;

	if (semicolon_count == 0)
	{
		mini_sh.cmd_tables = malloc(sizeof(char *) * 2);
		mini_sh.cmd_tables[0] = ft_strdup(mini_sh.line);
		mini_sh.cmd_tables[1] = NULL;
		return ;
	}
	else
	{
		split_cmd_tables2(semicolon_location, semicolon_count);
	}
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

void	process_cmd_tables()
{
	int	*semicolon_location;
	int semicolon_count;

	semicolon_location = malloc(sizeof(int ) * 2);
	semicolon_location[0] = '\0';

	semicolon_location = separator_location(semicolon_location, &semicolon_count);

	split_cmd_tables(semicolon_location, semicolon_count);



	free(semicolon_location);
}
