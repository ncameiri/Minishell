/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 02:27:55 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/23 17:13:14 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

void	ft_env()
{
	int	i;

	i = 0;
	if(!ft_strcmp(mini_sh.ls_start->content[0],"env"))
	{
		while(mini_sh.env[i])
		{
			printf("%s\n",mini_sh.env[i++]);
		}
	}
}
