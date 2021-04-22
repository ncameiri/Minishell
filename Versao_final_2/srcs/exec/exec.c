/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 01:09:53 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/22 03:41:41 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

void run_builtin()
{

	if(ft_strcmp(mini_sh.args[0],"absolute_path") == 0)
		ft_absolute_path();
	else if(ft_strcmp(mini_sh.args[0],"testing") == 0)
		ft_testing();


	// Vai adicionando estas conforme vais fazendo os builtins.

	/*if(ft_strcmp(mini_sh.args[0],"echo") == 0)
		ft_echo();
	else if(ft_strcmp(mini_sh.args[0],"cd") == 0)
		ft_cd();
	else if(ft_strcmp(mini_sh.args[0],"pwd") == 0)
		ft_pwd();
	else if(ft_strcmp(mini_sh.args[0],"export") == 0)
		ft_export();
	else if(ft_strcmp(mini_sh.args[0],"unset") == 0)
		ft_unset();
	else if(ft_strcmp(mini_sh.args[0],"env") == 0)
		ft_env();
	else if(ft_strcmp(mini_sh.args[0],"exit") == 0)
		ft_exit();
	else if(ft_strcmp(mini_sh.args[0],"absolute_path") == 0)
		ft_absolute_path();*/
}

void run_bin()
{
	pid_t	pid;
	char *path;

	if (ft_strcmp(mini_sh.args[0],"testing") == 0)
	{
		ft_testing();
		return ;
	}

	path = ft_strjoin("/usr/bin/", mini_sh.args[0]);

	pid=fork();

	if(pid == 0)
	{
		execve(path,mini_sh.args ,mini_sh.env);
		printf("%s: command not found\n", mini_sh.args[0]);
	}
	else if (pid > 0)
		wait(&pid);
	else if (pid < 0)
		write(1,"ERROR",5);

	free(path);
}

int	check_builtin()
{
	if(ft_strcmp(mini_sh.args[0],"echo") == 0)
		return(1);
	else if(ft_strcmp(mini_sh.args[0],"cd") == 0)
		return(1);
	else if(ft_strcmp(mini_sh.args[0],"pwd") == 0)
		return(1);
	else if(ft_strcmp(mini_sh.args[0],"export") == 0)
		return(1);
	else if(ft_strcmp(mini_sh.args[0],"unset") == 0)
		return(1);
	else if(ft_strcmp(mini_sh.args[0],"env") == 0)
		return(1);
	else if(ft_strcmp(mini_sh.args[0],"exit") == 0)
		return(1);
	else if(ft_strcmp(mini_sh.args[0],"absolute_path") == 0)
		return(1);
	else if(ft_strcmp(mini_sh.args[0],"testing") == 0)
		return(1);
	else
		return (0);
}

void exec_func()
{
	char	*path;
	int		builtin;

	builtin = check_builtin();

	if (builtin == 1 && mini_sh.testing == 0)
		run_builtin();
	else if (builtin == 0 || mini_sh.testing == 1)
		run_bin();
}
