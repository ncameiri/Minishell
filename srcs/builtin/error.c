/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 16:11:28 by tisantos          #+#    #+#             */
/*   Updated: 2021/05/13 17:03:43 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

void	ft_error(void)
{
	printf("%i: command not found\n", mini_sh.dollar_error);
	mini_sh.dollar_error = 127;
}
