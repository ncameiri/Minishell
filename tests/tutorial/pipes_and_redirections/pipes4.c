/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/02 18:10:10 by tisantos          #+#    #+#             */
/*   Updated: 2021/05/03 15:56:46 by tisantos         ###   ########.fr       */
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

	count = 2 ;
}

void execute()
{
	//save in/out

	int tmpin;
	int tmpout;

	tmpin = dup(0);
	tmpout = dup(1);

	//set initial input

	int fdin;

	//if (infile)




}



int main(int argc, char **argv, char **envp)
{
	set_args();

	return(0);
}

// https://www.youtube.com/watch?v=VzCawLzITh0