/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complicated_exec2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/09 14:50:29 by tisantos          #+#    #+#             */
/*   Updated: 2021/05/09 23:05:04 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

int	take_outfile(t_simplecommand *smp_cmd)
{
	int fd;
	int i;

	fd = 0;
	i = 0;
	while (smp_cmd->outfile[i] != NULL)
	{
		if (i > 0)
			close(fd);
		if (smp_cmd->append == 1)
			fd = open(smp_cmd->outfile[i], O_WRONLY | O_CREAT | O_APPEND, 0777);
		else
			fd = open(smp_cmd->outfile[i], O_WRONLY | O_CREAT | O_TRUNC, 0777);
		i++;
	}
	return (fd);
}

int	take_infile(t_simplecommand *smp_cmd)
{
	int i;
	int fd;

	i = 0;
	fd = 0;
	while (smp_cmd->infile[i] != NULL)
	{
		if (fd > 0)
			close(fd);
		fd = open(smp_cmd->infile[i], O_RDONLY);
		if (fd == -1)
		{
			printf("bash: %s: No such file or directory\n", smp_cmd->infile[i]);
			break;
		}
		i++;
	}

	return (fd);
}

int	check_outfile(t_simplecommand *smp_cmd)
{
	if (smp_cmd->outfile[0] != NULL)
		return (1);
	return (0);
}

int	check_infile(t_simplecommand *smp_cmd)
{
	if (smp_cmd->infile[0] != NULL)
		return (1);
	return (0);
}
