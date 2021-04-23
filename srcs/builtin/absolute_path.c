/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   absolute_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 02:53:37 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/23 02:28:00 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

void	ft_absolute_path()
{
	if (mini_sh.absolute_path == 1)
		mini_sh.absolute_path = 0;
	else
		mini_sh.absolute_path = 1;
}
