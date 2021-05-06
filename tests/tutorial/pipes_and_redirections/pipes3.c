/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/30 19:35:33 by tisantos          #+#    #+#             */
/*   Updated: 2021/05/02 16:45:02 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

char	*args1[5];
char	*args2[3];
char	*args3[2];
int		count;

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*string;
	int		len;
	int		i;
	int		b;

	i = 0;
	b = 0;
	if (!s1)
		return (NULL);
	if (!s2 && s1)
		return ((char *)s1);
	len = strlen(s1) + strlen(s2);
	string = malloc(len + 1 * sizeof(string));
	if (string == NULL)
		return (NULL);
	while (s1[i] != '\0')
	{
		string[i] = s1[i];
		i++;
	}
	while (s2[b] != '\0')
		string[i++] = s2[b++];
	string[i] = '\0';
	return (string);
}

void set_args()
{
	args1[0] = "ping";
	args1[1] = "-c";
	args1[2] = "1";
	args1[3] = "google.com";
	args1[4] = NULL;

	args2[0] = "grep";
	args2[1] = "rtt";
	args2[2] = NULL;

	args3[0] = "wc";
	args3[1] = NULL;

	count = 3 ;
}

int main(int argc, char **argv, char **envp)
{
	set_args();

	int fd[2];
	pid_t pid;

	int fd_backup;
	fd_backup = 0;

	char **args;

	int i = 0;

	while (i < count)
	{
		if (i == 0)
		{
			args = args1;
		}
		else if (i == 1)
		{
			args = args2;
		}
		else if (i == 2)
		{
			args = args3;
		}

		pipe(fd);
		pid = fork();
		if (pid == -1)
		{
			printf("ErrOr!");
			return (0);
		}
		else if (pid == 0)
		{
			dup2(fd_backup, 0);

			if (i != count - 1)
			{
				dup2(fd[1], 1);
			}
			close(fd[0]);

			char *path;
			path = ft_strjoin("/usr/bin/", args[0]);
			execve(path, args, envp);
			free(path);

			return(1);
		}
		else
		{
			wait(&pid);
			close(fd[1]);
			fd_backup = fd[0];
			i++;
		}
	}


	return (0);
}



// ping -c 1 google.com | grep rtt | wc

//	https://www.youtube.com/watch?v=NkfIUo_Qq4c

