/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_to_list4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/28 11:29:40 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/28 11:29:48 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

int	is_a_shell_symbol(char a, char a1)
{
	if (a == '>' && a1 == '>')
		return (1);
	if (a == '|')
		return (1);
	if (a == '<')
		return (1);
	if (a == '>')
		return (1);
	return (0);
}
int	chck_iespac(char s1, char s)
{
	if (!ft_strchr(SHELL_DELIMITERS, s1) && s == '\"')
		return (1);
	else if (!ft_strchr(SHELL_DELIMITERS, s1) && s == '\'')
		return (1);
	else
		return (0);
}
int	check_inside_symbols(t_splvariab *var, char *str)
{
	int	i;
	int	ch;

	ch = 0;
	i = 0;
	{
		/*while(str[i] && (str[i] == '\t' || str[i] == '\n' || str[i] == '\a' || str[i] == ' ' || str[i] == '\r'))
			i++;
		ch = is_a_shell_symbol(str[i],str[i+1]);
		if(str[i+1] == '>' && str[i+1]!= '\0')
			i++;
		while(str[i] && (str[i] == '\t' || str[i] == '\n' || str[i] == '\a' || str[i] == ' ' || str[i] == '\r'))
			i++;*/
		if (str[i+1] && is_a_shell_symbol(str[i], str[i + 1]))
			return (1);
	}
	return 0;
}
void	chck_dup_symbols(void)
{
	t_linklis	*lst;
	int			i;
	t_splvariab	var;

	var.double_q = 0;
	var.single_q = 0;
	lst = mini_sh.ls_start;
	while (lst)
	{
		i = 0;
		while (lst->content[i])
		{
			if (lst->content[i] && check_inside_symbols(&var, lst->content[i]))
				mini_sh.error = 1;
			i++;
		}
		lst = lst->next;
	}
}
void	chck_begend_symbols(void)
{
	t_linklis	*lst;
	int			i;
	char		*pt;

	lst = mini_sh.ls_start;
	while (lst)
	{
		pt = lst->pre_split;
		i = 0;
		if (pt[i])
		{
			while (pt[i] && ft_strchr(SHELL_DELIMITERS,pt[i]))
				i++;
			if (ft_strchr("|><", pt[i]))
				mini_sh.error = 1;
			i = ft_strlen(pt) - 1;
			while (pt[i] && i>0 && ft_strchr(SHELL_DELIMITERS,pt[i]))
				i--;
			if (ft_strchr("|><", pt[i]))
				mini_sh.error = 1;
		}
		else
			mini_sh.error =1;
		lst = lst->next;
	}
}
