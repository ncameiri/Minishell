/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_old.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 01:09:53 by tisantos          #+#    #+#             */
/*   Updated: 2021/05/10 16:06:54 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"



void	run_builtin()
{

	if(ft_strcmp(mini_sh.args[0],"absolute_path") == 0)
		ft_absolute_path();
	else if(ft_strcmp(mini_sh.args[0],"history") == 0)
		ft_history();
	else if(ft_strcmp(mini_sh.args[0],"testing") == 0)
		ft_testing();
	else if(ft_strcmp(mini_sh.args[0],"pwd") == 0)
		ft_pwd();
	/*else if(ft_strcmp(mini_sh.args[0],"echo") == 0)
		ft_echo(ft_echo);
	else if(ft_strcmp(mini_sh.args[0],"env") == 0)
		ft_env();
	else if(ft_strcmp(mini_sh.args[0],"cd") == 0)
		ft_cd();*/

	// Vai adicionando estas conforme vais fazendo os builtins.
	/*
	else if(ft_strcmp(mini_sh.args[0],"export") == 0)
		ft_export();
	else if(ft_strcmp(mini_sh.args[0],"unset") == 0)
		ft_unset();

	else if(ft_strcmp(mini_sh.args[0],"exit") == 0)
		ft_exit();
	*/
}

void	run_bin()
{
	char	*bin_path;

	if (ft_strcmp(mini_sh.args[0],"testing") == 0)
	{
		ft_testing();
		return ;
	}

	bin_path = ft_strjoin("/usr/bin/", mini_sh.args[0]);

	mini_sh.pid=fork();

	if(mini_sh.pid == 0)
	{
		if (ft_strchr(mini_sh.args[0], '/'))
			execve(mini_sh.args[0], mini_sh.args, mini_sh.env); // This one is in case it's to read an ./a.out or other executable.
		execve(bin_path, mini_sh.args, mini_sh.env); // If it's not an executable, you read from /usr/bin/
		printf("%s: command not found\n", mini_sh.args[0]);
		free(bin_path);

		exit(1);
	}
	else if (mini_sh.pid > 0)
		wait(&mini_sh.pid);
	else if (mini_sh.pid < 0)
		write(1,"ERROR",5);

	mini_sh.pid = 0;

	free(bin_path);
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
	else if(ft_strcmp(mini_sh.args[0],"history") == 0)
		return(1);
	else
		return (0);
}

void	exec()
{
	char	*path;
	int		builtin;

	builtin = check_builtin();

	if (builtin == 1 && mini_sh.testing == 0)
		run_builtin();
	else if (builtin == 0 || mini_sh.testing == 1)
		run_bin();
}


// NOT BEING USED! BUT DONT DELETE. YET.







