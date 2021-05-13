/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complicated_exec.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/07 21:31:30 by tisantos          #+#    #+#             */
/*   Updated: 2021/05/13 17:12:30 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

void	complicated_execute5(t_complicated_exec *norm)
{
	dup2(norm->tmpin, 0);
	dup2(norm->tmpout, 1);
	close(norm->tmpin);
	close(norm->tmpout);
	if (mini_sh.pid > 0)
		waitpid(mini_sh.pid, NULL, 0);
	else if (mini_sh.pid < 0)
		printf("ERROR\n");
}

void	complicated_execute4(t_simplecommand **simple_cmd,
							t_complicated_exec *norm)
{
	char	*bin_path;

	mini_sh.dollar_error = 0;
	mini_sh.pid = fork();
	if (mini_sh.pid == 0)
	{
		(*simple_cmd) = remove_quotation_marks((*simple_cmd));
		if ((*simple_cmd)->builtin == 1)
			run_builtin_complicated((*simple_cmd));
		else
		{
			if (ft_strchr((*simple_cmd)->command[0], '/'))
				execve((*simple_cmd)->command[0],
					(*simple_cmd)->command, mini_sh.env);
			bin_path = get_path((*simple_cmd)->command[0]);
			execve(bin_path, (*simple_cmd)->command, mini_sh.env);
			printf("%s: command not found\n", (*simple_cmd)->command[0]);
			mini_sh.dollar_error = errno;
			free(bin_path);
		}
		mini_sh.dollar_error = 127;
		exit_finale(1);
		exit(0);
	}
}

void	complicated_execute3(t_simplecommand **simple_cmd,
							t_complicated_exec *norm)
{
	if ((*simple_cmd)->next != NULL && (*simple_cmd)->outfile[0] == NULL)
	{
		pipe(norm->fdpipe);
		norm->fdout = norm->fdpipe[1];
		norm->fdin = norm->fdpipe[0];
	}
	dup2(norm->fdout, 1);
	close(norm->fdout);
}

void	complicated_execute2(t_simplecommand **simple_cmd,
							t_complicated_exec *norm)
{
	if (check_outfile((*simple_cmd)) == 1)
		norm->fdout = take_outfile((*simple_cmd));
	else if (check_outfile((*simple_cmd)) == 0)
		norm->fdout = dup(norm->tmpout);
	if (norm->fdout == -1)
	{
		printf("bash: %s: No such file or directory\n",
			(*simple_cmd)->outfile[0]);
	}
}

void	complicated_execute(t_simplecommand *simple_cmd)
{
	t_complicated_exec	norm;

	norm.tmpin = dup(0);
	norm.tmpout = dup(1);
	norm.fdin = dup(norm.tmpin);
	while (simple_cmd != NULL)
	{
		complicated_execute2(&simple_cmd, &norm);
		if (infile_stuff(&simple_cmd, &norm) == -1)
			return ;
		if (norm.fdout == -1)
			break ;
		complicated_execute3(&simple_cmd, &norm);
		if (check_builtin_no_fork(&simple_cmd) == 1)
			continue ;
		complicated_execute4(&simple_cmd, &norm);
		simple_cmd = simple_cmd->next;
	}
	complicated_execute5(&norm);
}
