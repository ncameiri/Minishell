/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 02:53:37 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/23 22:14:38 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

void	ft_history(void)
{
	int	i;

	i = 0;
	while (mini_sh.history[i] != NULL)
	{
		printf(" %d  %s\n", i + 1, mini_sh.history[i]);
		i++;
	}
}
