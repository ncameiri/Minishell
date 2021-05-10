/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_to_simple_cmd3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/07 04:22:41 by tisantos          #+#    #+#             */
/*   Updated: 2021/05/10 15:16:06 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

void	*ft_lstnew_simple_add3(t_simplecommand_temp add)
{
	mini_sh.simple_cmd->next = malloc(sizeof(t_simplecommand));
	mini_sh.simple_cmd = mini_sh.simple_cmd->next;
	if (mini_sh.simple_cmd == NULL)
		return (NULL);
	if (mini_sh.simple_cmd)
	{
		mini_sh.simple_cmd->command = add.temp_command;
		mini_sh.simple_cmd->infile = new_array_array(add.temp_infile);
		mini_sh.simple_cmd->outfile = new_array_array(add.temp_outfile);
		mini_sh.simple_cmd->builtin = add.temp_builtin;
		mini_sh.simple_cmd->outfiles = add.temp_outfiles;
		mini_sh.simple_cmd->infiles = add.temp_infiles;
		mini_sh.simple_cmd->append = add.temp_append;
		if (add.temp_infile_extra_text != NULL)
				mini_sh.simple_cmd->infile_extra_text = ft_strdup(add.temp_infile_extra_text);
		else
			mini_sh.simple_cmd->infile_extra_text = NULL;
		mini_sh.simple_cmd->next = NULL;
	}
}

void	*ft_lstnew_simple_add2(t_simplecommand_temp add)
{
	t_simplecommand *start;
	t_simplecommand *temp;
	int i;

	start = mini_sh.simple_cmd;
	temp = mini_sh.simple_cmd;
	i = 0;
	while (temp != NULL)
	{
		i++;
		temp = temp->next;
	}
	while (i > 1)
	{
		mini_sh.simple_cmd = mini_sh.simple_cmd->next;
		i--;
	}
	ft_lstnew_simple_add3(add);
	mini_sh.simple_cmd = start;
}

void	*ft_lstnew_simple_add1(t_simplecommand_temp add)
{
	mini_sh.simple_cmd->command = add.temp_command;
	mini_sh.simple_cmd->infile = new_array_array(add.temp_infile);
	mini_sh.simple_cmd->outfile = new_array_array(add.temp_outfile);
	mini_sh.simple_cmd->builtin = add.temp_builtin;
	mini_sh.simple_cmd->outfiles = add.temp_outfiles;
	mini_sh.simple_cmd->infiles = add.temp_infiles;
	mini_sh.simple_cmd->append = add.temp_append;
	if (add.temp_infile_extra_text != NULL)
		mini_sh.simple_cmd->infile_extra_text = ft_strdup(add.temp_infile_extra_text);
	else
		mini_sh.simple_cmd->infile_extra_text = NULL;
	mini_sh.simple_cmd->next = NULL;
}

void	*ft_lstnew_simple_add(t_simplecommand_temp add)
{
	if (mini_sh.simple_cmd == NULL)
	{
		mini_sh.simple_cmd = malloc(sizeof(t_simplecommand));
		if (mini_sh.simple_cmd == NULL)
			return (NULL);
		if (mini_sh.simple_cmd)
		{
			ft_lstnew_simple_add1(add);
		}
	}
	else
		ft_lstnew_simple_add2(add);
}
