/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/30 19:35:33 by tisantos          #+#    #+#             */
/*   Updated: 2021/05/02 10:22:30 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char **argv, char **envp)
{
	int fd[2];
	// fd[0] = read
	// fd[1] = write

	char *args1[5];
	args1[0] = "ping";
	args1[1] = "-c";
	args1[2] = "5";
	args1[3] = "google.com";
	args1[4] = NULL;

	char *args2[3];
	args2[0] = "grep";
	args2[1] = "rtt";
	args2[2] = NULL;

	pid_t id;
	pid_t id2;

	if (pipe(fd) == -1)
		return (0);

	id = fork();

	if (id == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		execve("/usr/bin/ping", args1, envp);
	}


	id2 = fork();

	if (id2 == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		execve("/usr/bin/grep", args2, envp);
	}


	close(fd[0]);
	close(fd[1]);

	wait(&id);
	wait(&id2);

	return (0);

}

// ping -c 5 google.com | grep rtt

//	https://www.youtube.com/watch?v=6xbLgZpOBi8

//	https://tldp.org/LDP/abs/html/io-redirection.html
