/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 17:12:50 by tisantos          #+#    #+#             */
/*   Updated: 2021/05/10 16:05:38 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

void	ft_putnstr(char *str, int n)
{
	int		i;

	i = -1;
	if (n < 0)
	{
		while (str[++i] && i < (int)ft_strlen(str) + n)
			write(1,&str[i],1);
	}
	else
	{
		while (str[++i] && i < n)
			write(1,&str[i],1);
	}
}

static void	echo_out(char **str, int pos)
{
	int		quote;
	//int		str_len;

	quote = (str[pos][0] == '"' || str[pos][0] == '\'');
	//str_len = (int)ft_strlen(str[pos]);
	//ends_with = (str[pos][str_len - 1] == '"');
	if (quote)
		ft_putnstr(str[pos] + 1, -1);
	else
		ft_putstr_fd(str[pos],1);
	if (str[pos + 1])
		write(1," ",1);
}

int	ft_echo(char **content)
{
	int		i;
	int		n_flag;

	n_flag = 0;
	if (!content[1])
	{
		write(1, "\n", 1);
		return (1);
	}
	else if (!ft_strcmp(content[1] ,"-n"))
		n_flag = 1;
	i = 0;
	if (n_flag)
		++i;
	while (content[++i])
	{
		if(ft_strlen(content[i]))
		echo_out(content, i);
		if (!content[i + 1] && !n_flag)
			write(1,"\n",1);
	}
	return (1);
}
