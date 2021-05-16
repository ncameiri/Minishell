/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complicated_exec4.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/16 04:03:15 by tisantos          #+#    #+#             */
/*   Updated: 2021/05/16 04:20:48 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

char	*get_path(char *command)
{
	char	*bin_path;

	if (g_sh.islinux == 1)
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
