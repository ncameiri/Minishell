/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/18 23:57:53 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/19 00:48:55 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char *lsh_read_line(void)
{
	char *line = NULL;
	ssize_t bufsize = 0; // have getline allocate a buffer for us

	if (getline(&line, &bufsize, stdin) == -1)
	{
		if (feof(stdin))
		{
			exit(EXIT_SUCCESS);  // We recieved an EOF
		}
		else
		{
			perror("readline");
			exit(EXIT_FAILURE);
		}
	}

	return line;
}


void lsh_loop(void)
{
	char *line;
	char **args;
	int status;

	do
	{
		printf("> ");
		line = lsh_read_line();
		//args = lsh_split_line(line);
		//status = lsh_execute(args);

		free(line);
		//free(args);
	} while (status);
}



int main (int argc, char **argv)
{

	// Load config files, if any.

	// Run command loop.
	//lsh_loop();

	// Perform any shutdown/cleanup.
	char *line = NULL;
	ssize_t bufsize = 0;
	//getline(&line, &bufsize, stdin);
	get_next_line(0, &line);

	return (EXIT_SUCCESS);
}
