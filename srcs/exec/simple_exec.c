/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/28 18:44:28 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/28 19:05:34 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

void	run_builtin_simple(t_linklis *list)
{

	if(ft_strcmp(list->content[0],"absolute_path") == 0)
		ft_absolute_path();
	else if(ft_strcmp(list->content[0],"history") == 0)
		ft_history();
	else if(ft_strcmp(list->content[0],"testing") == 0)
		ft_testing();
	else if(ft_strcmp(list->content[0],"pwd") == 0)
		ft_pwd();
	else if(ft_strcmp(list->content[0],"echo") == 0)
		ft_echo();
	else if(ft_strcmp(list->content[0],"env") == 0)
		ft_env();
	else if(ft_strcmp(list->content[0],"cd") == 0)
		ft_cd();

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

void	run_bin_simple(t_linklis *list)
{
	char	*bin_path;

	if (ft_strcmp(list->content[0],"testing") == 0)
	{
		ft_testing();
		return ;
	}

	bin_path = ft_strjoin("/usr/bin/", list->content[0]);

	mini_sh.pid=fork();

	if(mini_sh.pid == 0)
	{
		if (ft_strchr(list->content[0], '/'))
			execve(list->content[0], list->content, mini_sh.env); // This one is in case it's to read an ./a.out or other executable.
		execve(bin_path, list->content, mini_sh.env); // If it's not an executable, you read from /usr/bin/
		printf("%s: command not found\n", list->content[0]);
		free(bin_path);

		exit_cntrl_d(1);

		exit(0);
	}
	else if (mini_sh.pid > 0)
		wait(&mini_sh.pid);
	else if (mini_sh.pid < 0)
		write(1,"ERROR",5);

	mini_sh.pid = 0;

	free(bin_path);
}

void	simple_execute(t_linklis *list)
{

	
	if (list->builtin == 1 && mini_sh.testing == 0)
		run_builtin_simple(list);
	else if (list->builtin == 0 || mini_sh.testing == 1)
		run_bin_simple(list);


}
