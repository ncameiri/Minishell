/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   both.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 13:03:36 by tisantos          #+#    #+#             */
/*   Updated: 2021/05/05 12:36:35 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <limits.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define OKAY 1
#define ERROR 0
#define MAXLINE 200			/* Maximum length of input line */
#define MAXARG 20			/* Max number of args for each simple command */
#define PIPELINE 5			/* Max number of simple commands in a pipeline */
#define MAXNAME 100			/* Maximum length of i/o redirection filename */
#define OPEN_MAX 256		/* Maximum number of FDs */

char line[MAXLINE+1];		/* User typed input line */
char *lineptr;				/* Pointer to current position in line[] */
char avline[MAXLINE+1];		/* Argv strings taken from line[] */
char *avptr;				/* Pointer to current position in avline[] */
char infile[MAXNAME+1];		/* Input redirection filename */
char outfile[MAXNAME+1];	/* Ouput redirection filename */

int backgnd;				/* TRUE if & ends pipeline else FALSE */
int lastpid;				/* PID of last simple command in pipeline */
int append;					/* TRUE for append redirection (») else FALSE */

struct	cmd
{

	char *av[MAXARG];
	int infd;
	int outfd;

}		cmdlin[PIPELINE];	/* Argvs and fds, one per simple command */




/***************************************************************/
/*							Parse							   */
/***************************************************************/

void getname(char *name)
{
	int i;

	for (i = 0; i<MAXNAME; ++i)
	{
		switch (*lineptr)
		{
			case '>':
			case '<':
			case '|':
			case '&':
			case ' ':
			case '\n':
			case '\t':
				*name = '\0';
				return;

			default:
				*name++ = *lineptr++;
				break;
		}
	}
	*name = '\0';
}

int check(char *ptr)
{
	char *tptr;

	while (*lineptr== ' ' )
		lineptr++;

	tptr = lineptr;

	while (*ptr!='\0' && *ptr==*tptr)
	{
		ptr++;
		tptr++;
	}
	if (*ptr!='\0')
		return(FALSE);
	else
	{
		lineptr = tptr;
		return(TRUE);
	}
}


void forkexec(struct cmd *ptr)
{
	int pid;

	/* 1 */
	if (pid = fork())
	{
		/* 2 */
		if (backgnd==TRUE)
			printf("%d\n", pid);
		lastpid = pid;
	}
	else
	{
		/* 3 */
		if (ptr->infd==0 && backgnd==TRUE)
			ptr->infd = open("/dev/null", O_RDONLY);

		/* 4 */
		if (ptr->infd!=0)
		{
			close(0);
			dup(ptr->infd);
		}

		if (ptr->outfd!=1)
		{
			close(1);
			dup(ptr->outfd);
		}

		/* 5 */
		if (backgnd==FALSE)
		{
			signal (SIGINT, SIG_DFL);
			signal (SIGQUIT, SIG_DFL);
		}

		/* 6 */
		for (int i = 3; i<OPEN_MAX; ++i)
			close(i);

		/* 7 */
		execvp(ptr->av[0], ptr->av);
		exit(1);
	}
}

void execute(int j)
{
	int i, fd, fds[2];

	/* 1 */
	if (infile[0] !='\0')
		cmdlin[0].infd = open(infile, O_RDONLY);

	/* 2 */
	if (outfile[0] !='\0')
		if (append==FALSE)
			cmdlin[j-1].outfd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		else
			cmdlin[j-1].outfd = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0666);

	/* 3 */
	if (backgnd==TRUE)
		signal (SIGCHLD, SIG_IGN);
	else
		signal (SIGCHLD, SIG_DFL);

	/* 4 */
	for (i = 0; i <j; ++i)
	{
		/* 5 */
		if (i<j-1)
		{
			pipe (fds);
			cmdlin[i].outfd = fds[1];
			cmdlin[i+1].infd = fds[0];
		}

		/* 6 */
		forkexec(&cmdlin[i]);

		/* 7 */
		if ((fd = cmdlin[i].infd)!=0)
			close(fd);

		if ((fd = cmdlin[i].outfd)!=1)
			close(fd);
	}

	/* 8 */
	if (backgnd==FALSE)
		while (wait (NULL) != lastpid);
}

void command(int i)
{
	int j, flag, inword;

	for (j = 0; j < MAXARG - 1; ++j)
	{
		while (*lineptr==' ' || *lineptr=='\t')
			++lineptr;

		cmdlin[i].av[j] = avptr;
		cmdlin[i].av[j + 1] = NULL;

		for (flag = 0; flag==0;)
		{
			switch (*lineptr)
			{
				case '>':
				case '<':
				case '|':
				case '&':
				case '\n':
					if (inword==FALSE)
						cmdlin[i].av[j] = NULL;

					*avptr++ = '\0';
					return;
				case ' ':
				case '\t':
					inword = FALSE;
					*avptr++ = '\0';
					flag = 1;
					break;
				default:
					inword = TRUE;
					*avptr++ = *lineptr++;
					break;
			}
		}
	}
}

int parse(void)
{
	int i;

	// 1 //

	command(0);

	// 2 //
	if (check("<"))
		getname(infile);

	// 3 //
	for (i = 1; i<PIPELINE; ++i)
		if (check("|"))
			command(i);
		else
			break;

	// 4 //
	if (check(">"))
	{
		if (check(">"))
			append = TRUE;

		getname(outfile);
	}

	// 5 //
	if (check("&"))
		backgnd = TRUE;

	// 6 //
	if (check("\n"))
		return(i);
	else
	{
		fprintf(stderr, "Command line syntax error\n");
		return (ERROR);
	}
}




/***************************************************************/
/*							MAIN							   */
/***************************************************************/


//	stdin:  Same as fd = 0.		<--- This is what the keyboard writes. 			STDIN_FILENO
//	stdout: Same as fd = 1. 	<--- This is what the shell terminal shows.		STDOUT_FILENO
//	stderr: Same as fd = 2. 	<--- This is what the shell terminal shows.		STDERR_FILENO

int get_line(void)
{
	int i;

	for (i = 0; (line[i] = getchar())!='\n' && i<MAXLINE; ++i);

	if (i == MAXLINE)
	{
		fprintf(stderr, "Command line too long\n");
		return(ERROR);
	}
	line[i + 1] = '\0';

	return (OKAY);
}

void	initwarm(void)
{
	int i;

	backgnd = FALSE;
	lineptr = line;
	avptr = avline;
	infile[0] = '\0';
	outfile[0] = '\0';
	append = FALSE;
	for (i = 0; i < PIPELINE; ++i)		// Sets all the 5 structs of cmdlin to
	{									// infd = 0 and outfd = 1
		cmdlin[i].infd = 0;
		cmdlin[i].outfd = 1;
	}

	for (i = 3; i < OPEN_MAX; ++i)		// Closes every FD except 0 1 2
		close(i);

	printf("tsh: ");
	fflush(stdout);
}

int main (void)
{
/*
	int i;

	while(1)
	{
		initwarm();

		if (get_line())
			if (i = parse())
			{
				execute(i);
			}
	}
*/
	int a = open("aqui.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	int b = open("alo.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);

	//0
	//1
	//2
	//3
	//4

	int guardar_stdout;

	guardar_stdout = dup(STDOUT_FILENO);

	printf("%d\n", a);
	printf("%d\n", b);

	dup2(b, 1);

	printf("ola\n");

	dup2(guardar_stdout, STDOUT_FILENO);

	printf("ALOOO\n");


}
