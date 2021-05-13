/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_tables4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/11 17:53:36 by tisantos          #+#    #+#             */
/*   Updated: 2021/05/11 18:03:07 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

void	syntax_error(int value)
{
	if (value == 1)
		printf("%s\n", "bash: syntax error near unexpected token `;'");
	else if (value == 2)
		printf("%s\n", "bash: syntax error near unexpected token `;;'");
}

void	remove_cmd_blanks2(void)
{
	t_norminette_struct	n;
	char				*temp;

	n.i = 0;
	n.a = 0;
	n.c = 0;
	while (mini_sh.cmd_tables[n.i] != NULL)
	{
		n.copy = 0;
		if (mini_sh.cmd_tables[n.i][0] == ' ')
		{
			n.a = 0;
			while (mini_sh.cmd_tables[n.i][n.a] == ' '
				&& mini_sh.cmd_tables[n.i][n.a] != '\0')
			{
				n.a++;
				n.copy++;
			}
			temp = ft_strdup(mini_sh.cmd_tables[n.i] + n.copy);
			free(mini_sh.cmd_tables[n.i]);
			mini_sh.cmd_tables[n.i] = ft_strdup(temp);
			free(temp);
		}
		n.i++;
	}
}
