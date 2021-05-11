/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complicated_exec.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/07 21:31:30 by tisantos          #+#    #+#             */
/*   Updated: 2021/05/11 02:09:11 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

t_simplecommand	*remove_quotation_marks(t_simplecommand *simple_cmd)
{
	int i;
	char *temp;
	int a;
	int b;

	i = 0;
	a = 0;
	b = 1;
	temp = NULL;

	while(simple_cmd->command[i] != NULL)
	{
		if(simple_cmd->command[i][0] == '\"')
		{
			temp = malloc(sizeof(char) * ft_strlen(simple_cmd->command[i]));
			while (a < ft_strlen(simple_cmd->command[i]) - 2)
			{
				temp[a] = simple_cmd->command[i][b];
				a++;
				b++;
			}
			temp[a] = '\0';
			free(simple_cmd->command[i]);
			simple_cmd->command[i] = ft_strdup(temp);
			free(temp);
			a = 0;
			b = 1;
		}
		i++;
	}

	return(simple_cmd);
}

int	check_builtin_no_fork(t_simplecommand *lista)
{
	if (ft_strcmp(lista->command[0], "cd") == 0)
	{
		ft_cd(lista->command);
		return(1);
	}
	else if (ft_strcmp(lista->command[0], "env") == 0)
	{
		ft_env();
		return(1);
	}
	else if (ft_strcmp(lista->command[0], "exit") == 0)
	{
		ft_exit();
		return(1);
	}
	else if (ft_strcmp(lista->command[0], "export") == 0)
	{
		ft_export(lista->command);
		return(1);
	}
	else if (ft_strcmp(lista->command[0], "unset") == 0)
	{
		ft_unset(lista->command);
		return(1);
	}
	return(0);
}

void	run_builtin_complicated(t_simplecommand *lista)
{
	if (ft_strcmp(lista->command[0], "echo") == 0)
		ft_echo(lista->command);
	else if (ft_strcmp(lista->command[0], "pwd") == 0)
		ft_pwd();
}

void	complicated_execute(t_simplecommand *simple_cmd)
{
	t_complicated_exec norm;
	char				*bin_path;

	norm.tmpin = dup(0);
	norm.tmpout = dup(1);

	// set the initial input

	if(check_infile(simple_cmd) == 1)
	{
		norm.fdin = take_infile(simple_cmd);
		if (norm.fdin == -1)
		{
			simple_cmd = simple_cmd->next;
			if (simple_cmd != NULL)
				complicated_execute(simple_cmd);
			return ;
		}
	}
	else // Use default input
		norm.fdin = dup(norm.tmpin);

	while (simple_cmd != NULL)
	{
		// redirect input
		dup2(norm.fdin, 0);
		if (norm.fdin != -1)
			close(norm.fdin);
		//setup output

		if (check_outfile(simple_cmd) == 1 && simple_cmd->next != NULL)
				norm.fdout = take_outfile(simple_cmd);
		else if (simple_cmd->next == NULL)
		{
				//last simple command
			if (check_outfile(simple_cmd) == 1)
				norm.fdout = take_outfile(simple_cmd);
			else // use default output
				norm.fdout = dup(norm.tmpout);
			if (norm.fdout == -1)
			{
				printf("bash: %s: No such file or directory\n", simple_cmd->outfile[0]);
				break;
			}
		}
		else
		{
			// not last simple command
			pipe(norm.fdpipe);
			norm.fdout = norm.fdpipe[1];
			norm.fdin = norm.fdpipe[0];
		}

		dup2(norm.fdout,1);
		close(norm.fdout);

		if(check_builtin_no_fork(simple_cmd) == 1)
		{
			simple_cmd = simple_cmd->next;
			continue;
		}
		mini_sh.pid = fork();
		if (mini_sh.pid == 0)
		{
			simple_cmd = remove_quotation_marks(simple_cmd);
			if (simple_cmd->builtin == 1)
				run_builtin_complicated(simple_cmd);
			else
			{
				bin_path = ft_strjoin("/usr/bin/", simple_cmd->command[0]);
				execve(bin_path, simple_cmd->command, mini_sh.env);
				printf("%s: command not found\n", simple_cmd->command[0]);
				free(bin_path);
			}
			exit_cntrl_d(1);
			exit(0);
		}

		simple_cmd = simple_cmd->next;
	}
	dup2(norm.tmpin, 0);
	dup2(norm.tmpout, 1);
	close(norm.tmpin);
	close(norm.tmpout);

	if (mini_sh.pid > 0)
		waitpid(mini_sh.pid, NULL, 0);
	else if (mini_sh.pid < 0)
		printf("ERROR\n");

}
