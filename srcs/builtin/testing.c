/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 03:32:21 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/22 03:38:06 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

void	ft_testing()
{
	if (mini_sh.testing == 1)
	{
		printf("testing: OFF\n");
		mini_sh.testing = 0;
	}
	else
	{
		printf("testing: ON\n");
		mini_sh.testing = 1;
	}
}
