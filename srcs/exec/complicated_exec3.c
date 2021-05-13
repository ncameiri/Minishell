/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complicated_exec3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/11 21:26:20 by tisantos          #+#    #+#             */
/*   Updated: 2021/05/13 16:19:24 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

char	*get_path(char *command)
{
	char	*bin_path;

	if (mini_sh.islinux == 1)
	{
		bin_path = ft_strjoin("/usr/bin/", command);
		return (bin_path);
	}
	if (ft_strcmp(command, "cat") == 0 || ft_strcmp(command, "chmod") == 0
		|| ft_strcmp(command, "cp") == 0 || ft_strcmp(command, "date") == 0
		|| ft_strcmp(command, "hostname") == 0
		|| ft_strcmp(command, "kill") == 0
		|| ft_strcmp(command, "link") == 0 || ft_strcmp(command, "ls") == 0
		|| ft_strcmp(command, "mkdir") == 0 || ft_strcmp(command, "ps") == 0
		|| ft_strcmp(command, "rm") == 0 || ft_strcmp(command, "rmdir") == 0
		|| ft_strcmp(command, "sleep") == 0 || ft_strcmp(command, "stty") == 0
		|| ft_strcmp(command, "zsh") == 0)
	{
		bin_path = ft_strjoin("/bin/", command);
		return (bin_path);
	}
	else
	{
		bin_path = ft_strjoin("/usr/bin/", command);
		return (bin_path);
	}
}

t_simplecommand	*remove_quotation_marks(t_simplecommand *simple_cmd)
{
	t_norminette_struct	n;
	char				*temp;

	n.i = 0;
	n.a = 0;
	n.b = 1;
	temp = NULL;
	while (simple_cmd->command[n.i] != NULL)
	{
		if (simple_cmd->command[n.i][0] == '\"')
		{
			temp = malloc(sizeof(char) * ft_strlen(simple_cmd->command[n.i]));
			while (n.a < ft_strlen(simple_cmd->command[n.i]) - 2)
				temp[n.a++] = simple_cmd->command[n.i][n.b++];
			temp[n.a] = '\0';
			free(simple_cmd->command[n.i]);
			simple_cmd->command[n.i] = ft_strdup(temp);
			free(temp);
			n.a = 0;
			n.b = 1;
		}
		n.i++;
	}
	return (simple_cmd);
}

int	check_builtin_no_fork2(t_simplecommand **lista)
{
	int	i;

	i = 0;
	if (ft_strcmp((*lista)->command[0], "export") == 0)
	{
		ft_export((*lista)->command);
		(*lista) = (*lista)->next;
		i = 1;
	}
	else if (ft_strcmp((*lista)->command[0], "unset") == 0)
	{
		ft_unset((*lista)->command);
		(*lista) = (*lista)->next;
		i = 1;
	}
	else if (ft_strcmp((*lista)->command[0], "$?") == 0)
	{
		ft_error();
		(*lista) = (*lista)->next;
		i = 1;
	}
	return (i);
}

int	check_builtin_no_fork(t_simplecommand **lista)
{
	int	i;

	i = 0;
	if (ft_strcmp((*lista)->command[0], "cd") == 0)
	{
		ft_cd((*lista)->command);
		(*lista) = (*lista)->next;
		return (1);
	}
	else if (ft_strcmp((*lista)->command[0], "env") == 0)
	{
		ft_env();
		(*lista) = (*lista)->next;
		return (1);
	}
	else if (ft_strcmp((*lista)->command[0], "exit") == 0)
	{
		ft_exit((*lista)->command);
		(*lista) = (*lista)->next;
		return (1);
	}
	i = check_builtin_no_fork2(lista);
	return (i);
}

void	run_builtin_complicated(t_simplecommand *lista)
{
	if (ft_strcmp(lista->command[0], "echo") == 0)
		ft_echo(lista->command);
	else if (ft_strcmp(lista->command[0], "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(lista->command[0], "history") == 0)
		ft_history();
}
