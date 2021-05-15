/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcaps.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 23:50:52 by tisantos          #+#    #+#             */
/*   Updated: 2021/05/15 19:37:25 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

int		ft_putint(int c)
{
	return (write(1, &c, 1));
}

int		set_capabilities()
{
	mini_sh.keys_on = tgetstr("ks", &mini_sh.buffer);
	if (mini_sh.keys_on)
		tputs(mini_sh.keys_on, 1, ft_putint);
	mini_sh.keys_off = tgetstr("ke", &mini_sh.buffer);
	mini_sh.up_arrow = tgetstr("ku", &mini_sh.buffer);
	mini_sh.down_arrow = tgetstr("kd", &mini_sh.buffer);
	if (mini_sh.islinux == 1)
		mini_sh.backspace = tgetstr("kb", &mini_sh.buffer);
	else
		mini_sh.backspace = ft_strdup("\x7f");
	mini_sh.del_line = tgetstr("dl", &mini_sh.buffer);
	mini_sh.cursor_left = tgetstr("cr", &mini_sh.buffer);
	if ((!mini_sh.keys_on) || (!mini_sh.up_arrow) || (!mini_sh.down_arrow)
		|| (!mini_sh.backspace) || (!mini_sh.del_line) || (!mini_sh.keys_off)
		|| (!mini_sh.cursor_left))
		return (0);
	else
		return (1);
}

char	*get_termtype()
{
	int		i;
	char	*temp;

	i = 0;
	temp = NULL;
	while (mini_sh.env[i] != NULL)
	{
		if (ft_strncmp(mini_sh.env[i], "TERM=", 5) == 0)
		{
			mini_sh.env[i] += 5;
			temp = mini_sh.env[i];
			mini_sh.env[i] -= 5;
			break;
		}
		i++;
	}
	return (temp);
}

void	init_termcaps()
{
	char	*term_type;
	int		ret;

	if (isatty(STDIN_FILENO) != 1)
		exit(EXIT_FAILURE);
	if (tcgetattr(STDIN_FILENO, &mini_sh.old_term) == -1)
		exit(EXIT_FAILURE);
	if (mini_sh.islinux == 1)
	{
		mini_sh.buffer = ft_calloc(2048, 1);
		if (mini_sh.buffer == NULL)
			exit(EXIT_FAILURE);
	}
	term_type = get_termtype();
	if (term_type == NULL)
		exit(EXIT_FAILURE);
	ret = tgetent(mini_sh.buffer, term_type);
	if (ret <= 0)
		exit(EXIT_FAILURE);
	else if (set_capabilities() == 0)
		exit(EXIT_FAILURE);
}
