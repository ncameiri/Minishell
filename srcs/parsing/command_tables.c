/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_tables.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/24 00:36:21 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/24 01:39:13 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

void	split_cmd_tables2(int *semicolon_location, int semicolon_count)
{
	int i;
	int a;
	char *temp;
	int	*lens;

	i = 0;
	a = 0;
	mini_sh.cmd_tables = malloc(sizeof(char *) * (semicolon_count + 1));
	lens = malloc(sizeof(int) * (semicolon_count + 1));
	if (mini_sh.cmd_tables == NULL || lens == NULL)
		return ;

	lens[i++] = (semicolon_location[i] - 0);
	while (i < semicolon_count - 1)
	{
		lens[i] = (semicolon_location[i + 1] - semicolon_location[i]);
		i++;
	}
	lens[i] = (ft_strlen(mini_sh.line - 1) - semicolon_location[i]);

	i = 0;

	printf("aqui 0 %d\n", lens[0]);
	printf("aqui 1 %d\n", lens[1]);
	printf("aqui 2 %d\n", lens[2]);

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
