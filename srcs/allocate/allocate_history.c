/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocate_history.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 21:40:49 by tisantos          #+#    #+#             */
/*   Updated: 2021/05/13 15:40:18 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

void	save_history(void)
{
	int	i;

	i = 0;
	if (mini_sh.history == NULL)
	{
		if (mini_sh.line != NULL || mini_sh.line[0] != '\0')
		{
			mini_sh.history = malloc(sizeof(char *) * 2);
			if (mini_sh.history == NULL)
				return ;
			mini_sh.history[0] = ft_strdup(mini_sh.line);
			mini_sh.history[1] = NULL;
			mini_sh.history_len = 2;
		}
	}
	else
	{
		if (mini_sh.line[0] != '\0'
			&& ft_strcmp(mini_sh.history[mini_sh.history_len - 2],
				mini_sh.line) != 0)
		{
			mini_sh.history = add_str_to_arrarr(mini_sh.history, mini_sh.line);
			mini_sh.history_len++;
		}
	}
}
