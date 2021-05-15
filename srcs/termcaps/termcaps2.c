/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcaps2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/15 02:27:42 by tisantos          #+#    #+#             */
/*   Updated: 2021/05/15 21:52:28 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

int		has_history()
{
	if (mini_sh.history_len == 0)
		return(0);
	else
		return(1);
}

void	parse_input_history(char *buf, int *i)
{
	char	*input;
	int		only_up_arrow;

	only_up_arrow = 0;

	if (mini_sh.current_history == mini_sh.history_len)
		only_up_arrow = 1;
	if (has_history() == 0)
	{
		ft_bzero(&buf[*i], BUFSIZ - *i);
		return ;
	}
	if (ft_strcmp(mini_sh.up_arrow, &buf[*i]) == 0)
	{
		if (mini_sh.current_history > 0)
			mini_sh.current_history--;
		input = mini_sh.history[mini_sh.current_history];
		tputs(mini_sh.cursor_left, 1, ft_putint);
		tputs(mini_sh.del_line, 1, ft_putint);
		shell_prompt_line();
		ft_bzero(buf, BUFSIZ);
		ft_strcpy(buf, input);
		*i = write(STDOUT_FILENO, buf, ft_strlen(buf));
	}
	else if (ft_strcmp(mini_sh.down_arrow, &buf[*i]) == 0 && only_up_arrow == 0)
	{
		if (mini_sh.current_history < mini_sh.history_len - 1)
			mini_sh.current_history++;
		input = mini_sh.history[mini_sh.current_history];
		tputs(mini_sh.cursor_left, 1, ft_putint);
		tputs(mini_sh.del_line, 1, ft_putint);
		shell_prompt_line();
		ft_bzero(buf, BUFSIZ);
		ft_strcpy(buf, input);
		*i = write(STDOUT_FILENO, buf, ft_strlen(buf));
	}
}

void	turn_off_canonical_mode()
{
	mini_sh.new_term = mini_sh.old_term;
	mini_sh.new_term.c_lflag &= ~ICANON;
	mini_sh.new_term.c_lflag &= ~ECHO;
	mini_sh.new_term.c_lflag &= ~ISIG;
	mini_sh.new_term.c_cc[VMIN] = 1;
	mini_sh.new_term.c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &mini_sh.new_term) == -1)
	{
		exit_finale(1);
		exit(errno);
	}

}

void	turn_on_canonical_mode()
{
	if (tcsetattr(STDIN_FILENO, TCSANOW, &mini_sh.old_term) == -1)
	{
		exit_finale(1);
		exit(errno);
	}
}
