/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 01:46:30 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/22 02:28:16 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"
int		ft_pwd()
{
	char	cwd[1001];
    
	if (getcwd(cwd, 1000))
	{
		ft_putendl_fd(cwd, 1);
		return (1);
	}
	else
		return (-1); // em caso de erro
}


/*
echo
-n
"ola; > ;adeeus"
ola
|
cat
-e
>
file.txt
;
echo
ola

-pipe on ou off
- seta de file???
-contar ; fora das aspas

*/