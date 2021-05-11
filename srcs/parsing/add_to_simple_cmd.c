/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_to_simple_cmd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/07 04:20:00 by tisantos          #+#    #+#             */
/*   Updated: 2021/05/10 23:34:52 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

void	if_redirections_3(t_simplecommand_temp temp)
{
	if (temp.temp_outfile != NULL)
		free_array(temp.temp_outfile);
	if (temp.temp_infile != NULL)
		free_array(temp.temp_infile);
}

t_simplecommand_temp if_redirections_2(t_simplecommand_temp temp, t_linklis *list)
{
	int i;

	i = 0;
	temp.temp_command = malloc(sizeof(char *) * 60);
	while (list->content[i] != NULL)
	{
		temp.temp_command[i] = ft_strdup(list->content[i]);
		i++;
	}
	temp.temp_command[i] = NULL;

	temp.temp_infile = NULL;
	temp.temp_outfile = NULL;
	temp.temp_builtin = list->builtin;
	temp.temp_outfiles = 0;
	temp.temp_infiles = 0;
	temp.temp_append = 0;

	return (temp);
}

t_linklis  *if_redirections(t_linklis *list)
{
	t_simplecommand_temp temp;
	int iterations;
	int i;
	int a;
	int b;

	temp = if_redirections_2(temp, list);
	iterations = iterations_in_simple_command(list);
	i = 1;
	a = 0;
	b = 0;
	while(i < iterations)
	{
		if (list->type == 3 || list->type == 2) // > ou >> outfile
			list = if_redirections_outfile(list, &temp, a++);
		else if (list->type == 4) // < infile
			list = if_redirections_infile(list, &temp, b++);
		i++;
	}
	list = list->next;
	ft_lstnew_simple_add(temp);
	if_redirections_3(temp);
	return (list);
}

t_linklis	*if_no_redirections(t_linklis *list)
{
	t_simplecommand_temp temp;

	int i;

	i = 0;
	temp.temp_command = malloc(sizeof(char *) * 60);
	while (list->content[i] != NULL)
	{
		temp.temp_command[i] = ft_strdup(list->content[i]);
		i++;
	}
	temp.temp_command[i] = NULL;

	temp.temp_infile = NULL;
	temp.temp_outfile = NULL;
	temp.temp_builtin = list->builtin;
	temp.temp_outfiles = 0;
	temp.temp_infiles = 0;
	temp.temp_append = 0;

	ft_lstnew_simple_add(temp);

	list = list->next;

	return (list);
}

void	add_to_simple_commands_list()
{
	t_linklis *list;
	list = mini_sh.ls_start;

	mini_sh.simple_cmd = NULL;

	while (list != NULL)
	{											// "echo ola" ou "echo ola > alo | echo ola"
		if (list->type == 1 || list->type == 8) // Se houver só um simple command.
		{										// Ou se for só 1 pipe sem redirections.
			list = if_no_redirections(list);
		}
		else
		{									// "echo ola > aqui bla < ali bla"
			list = if_redirections(list);	// Se tiver redirections.
		}
	}
}
