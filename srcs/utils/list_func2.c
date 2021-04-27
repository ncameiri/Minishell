#include "../../shell.h"
int	is_separator(char check, char check2, int *type)
{
	if (check == '|')
	{
		*type = 1;
		return (1);
	}
	else if (check == '>' && check2 == '>')
	{
		*type = 2;
		return (1);
	}
	else if (check == '>')
	{
		*type = 3;
		return (1);
	}
	else if (check == '<')
	{
		*type = 4;
		return (1);
	}
	else
		return (0);
}

void	ft_lstspli(void)
{
	t_linklis	*lst;

	lst = mini_sh.ls_start;
	while (lst)
	{
		lst->content = ft_split_igquo(lst->pre_split, " ");
		lst = lst->next;
	}
}

void	ft_linklstclear(t_linklis **lst)
{
	t_linklis	*current;
	t_linklis	*next;

	if (!lst)
		return ;
	current = *lst;
	while (current != NULL)
	{
		next = current->next;
		free_array(current->content);
		free(current->pre_split);
		free(current);
		current = next;
	}
	*lst = NULL;
}

void add_var_init(t_var_add_tlis *va)
{
    va->i = 0;
	va->start = 0;
	va->last = 0;
	va->temp = NULL;
	mini_sh.error = 0;
}