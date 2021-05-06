/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/30 19:35:33 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/30 20:18:16 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
	int fd[2];
	// fd[0] - read
	// fd[1] - write

	pid_t id;


	if (pipe(fd) == -1)
	{
		printf("Error opening the pipe!\n");
		return (0);
	}

	id = fork();

	if (id == 0)
	{
		close(fd[0]);
		int x;
		printf("Input a number: ");
		scanf("%d", &x);
		write(fd[1], &x, sizeof(int));
		close(fd[1]);
		exit(0);
	}
	else
	{
		wait(&id);
		close(fd[1]);
		int y;
		read(fd[0], &y, sizeof(int));
		close(fd[0]);
		printf("%d\n", y);

	}

	return (0);

}
