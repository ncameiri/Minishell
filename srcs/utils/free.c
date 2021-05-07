/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/28 15:43:44 by tisantos          #+#    #+#             */
/*   Updated: 2021/05/07 04:43:13 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

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

void	exit_cntrl_d(int free_list)
{
	free_array(mini_sh.env);
	free(mini_sh.line);

	if (mini_sh.history_len > 0)
		free_array(mini_sh.history);
	if (mini_sh.cmd_tables != NULL)
		free_array(mini_sh.cmd_tables);

	if (free_list == 1)
		ft_linklstclear(&mini_sh.ls_start);

	if (mini_sh.simple_cmd != NULL)
		ft_lstclear_simple_struct(&mini_sh.simple_cmd);
}

// Meter o free dos links aqui.
