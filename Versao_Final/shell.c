/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/18 23:57:53 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/20 16:08:14 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
void own_func()
{
	int	i;
	
	i = 0;
	if(!ft_strcmp(mini_sh.args[0],"env"))
	{
		while(mini_sh.env[i])
		{
			printf("%s\n",mini_sh.env[i++]);
		}
	}
}
char *chck_built()
{
	if(!ft_strcmp(mini_sh.args[0],"ls"))
		return("/bin/ls");
	if(!ft_strcmp(mini_sh.args[0],"echo"))
		return("/bin/echo");
	if(!ft_strcmp(mini_sh.args[0],"pwd"))
		return("/bin/pwd");
	else 
		return NULL;
	
}

void exec_func()
{
	char *path;
	path = chck_built();
	execve(path,mini_sh.args ,mini_sh.env);	



}



char **shell_split_arguments(char *line)
{
	char **args;

	args = NULL;
	args = ft_split_chars(line, SHELL_DELIMITERS);

	return (args);
}

char *shell_prompt()
{
	char *line;
	char buf[4096 + 1];
	char *path;

	line = NULL;
	path = getcwd(buf, 4096);

	write(1, "\x1b[33m", 6);
	write(1, path, ft_strlen(path));
	write(1, "\x1b[0m", 5);
	write(1, "$ ", 3);

	get_next_line(0, &line);

	return (line);
}

void shell_loop()
{	
	pid_t pid;
	mini_sh.status=1;
	do
	{
		mini_sh.line = shell_prompt();
		mini_sh.args = shell_split_arguments(mini_sh.line);
		//status = shell_execute(args);
		pid=fork();

		if(pid == 0)
		{
			if(chck_built())
				exec_func();
			else
				own_func();
		
		}
		
		else if (pid > 0)//father
			wait(&pid);
		else if (pid < 0)
			write(1,"ERROR",5);
		//kill(pid,SIGCHLD);
		free(mini_sh.line);
		free_array(mini_sh.args);
	} while (mini_sh.status);//Substituir por 0 quando exit
}



int main (int argc, char **argv, char **env)
{

	mini_sh.env = env;
	shell_loop();

	return (EXIT_SUCCESS);
}
