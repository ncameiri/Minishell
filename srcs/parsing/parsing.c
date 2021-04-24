/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 17:00:42 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/24 00:50:40 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

int	check_complete_quotation()
{
	int i;
	int single_q;
	int double_q;

	i = 0;
	single_q = 0;
	double_q = 0;
	while (mini_sh.line[i] != '\0')
	{
		if (mini_sh.line[i] == '\"' && double_q == 0 && single_q == 0)
			double_q = 1;
		else if (mini_sh.line[i] == '\"' && double_q == 1 && single_q == 0)
			double_q = 0;
		else if (mini_sh.line[i] == '\'' && single_q == 0 && double_q == 0)
			single_q = 1;
		else if (mini_sh.line[i] == '\'' && single_q == 1 && double_q == 0)
			single_q = 0;
		i++;
	}
	if (single_q + double_q == 0)
		return (1);
	else
		return (0);
}

int	parsing()
{
	if (check_complete_quotation() == 0) // (Input is missing a " or '". It's invalid.).
	{
		printf("%s\n", "quotation mark incomplete");
		free_global("line", "empty", "empty", "empty");
		return 0;
	}
	process_cmd_tables();
	return (0);
}
