#include "../../shell.h"

t_linklis	*ft_linknew(char *pre_split, int type)
{
	t_linklis	*new;
	int			i;
	int			no_args;

	i = 0;
	new = malloc(sizeof(t_linklis));
	if (new == NULL)
		return (NULL);
	if (new)
	{
		new->pre_split = pre_split;
		new->type = type;
		new->next = NULL;
	}
	return (new);
}

void	ft_linkadd_back(t_linklis **lst, t_linklis *new)
{
	t_linklis		*begin;

	if (!*lst)
	{
		(*lst) = new;
		return ;
	}
	if (lst && (*lst) && new)
	{
		begin = (*lst);
		if (begin == NULL)
			(*lst) = new;
		else
		{
			while (begin->next)
				begin = begin->next;
			begin->next = new;
		}
	}
}

void	ft_lsttrim(void)
{
	t_linklis	*lst;
	int			i;
	char		*tmp;

	lst = mini_sh.ls_start;
	while (lst)
	{	
		i = 0;
		while (lst->content[i])
		{
			if (lst->content[i][0] == '\'')
				tmp = ft_strtrim(lst->content[i], "\'");
			else if (lst->content[i][0] == '\"')
				tmp = ft_strtrim(lst->content[i], "\"");
			else
				tmp = ft_strdup(lst->content[i]);
			free (lst->content[i]);
			lst->content[i] = tmp;
			i++;
		}
		lst = lst->next;
	}
}

int	sep_link(int index, int *a, int *type)
{
	t_var_seplink	va;

	va.i = *a;
	va.s = 0;
	va.single_q = 0;
	va.double_q = 0;
	va.n = mini_sh.cmd_tables[index][va.i];
	while (va.n != '\0')
	{
		*a = va.i;
		if (va.n == '\"' && va.double_q == 0 && va.single_q == 0 )
			va.double_q = 1;
		else if (va.n == '\"' && va.double_q == 1 && va.single_q == 0 )
			va.double_q = 0;
		else if (va.n == '\'' && va.single_q == 0 && va.double_q == 0)
			va.single_q = 1;
		else if (va.n == '\'' && va.single_q == 1 && va.double_q == 0 )
			va.single_q = 0;
		else if (is_separator(va.n, mini_sh.cmd_tables[index][va.i + 1],
			type) && va.single_q == 0 && va.double_q == 0)
			return (va.i);
		va.i++;
		va.n = mini_sh.cmd_tables[index][va.i];
	}
	return (0);
}

int	add_to_list(int index)
{
	t_var_add_tlis	va;

	add_var_init(&va);
	while (mini_sh.cmd_tables[index][va.i])
	{
		va.last = sep_link(index, &va.i, &va.type);
		if (va.last > 0 && (va.last - va.start) > 1)
		{	
			va.aux = ft_substr(mini_sh.cmd_tables[index],
					va.start, va.last - va.start);
			ft_linkadd_back(&mini_sh.ls_start, ft_linknew(va.aux, va.type));
			va.start = va.last + 1;
			if (va.type == 2)
				va.start += 1;
		}
		if (!mini_sh.cmd_tables[index][va.i + 1])
		{
			va.type = 8;
			va.aux = ft_substr(mini_sh.cmd_tables[index],
					va.start, (va.i + 1) - va.start);
			ft_linkadd_back(&mini_sh.ls_start, ft_linknew(va.aux, va.type));
		}
		va.i++;
	}

	ft_lstspli();
		chck_dup_symbols();
	
	
	chck_begend_symbols();
	ft_lsttrim();

	
	
if(mini_sh.error==1)
printf("DEU ERRO\n");
/*
ISTO E PARA VER OS ELEM PRE_SPLIT
*/
	t_linklis *ptr;
			ptr = mini_sh.ls_start;
			while(ptr)
			{printf("*type %d*%s*\n",ptr->type,ptr->pre_split);

			ptr=ptr->next;}
//PARA CIMA E TEMPORARIO
//ft_lstspli();//<--- IMPORTANTE
//ft_lsttrim();
printf("ORGANIZADA\n");
			ptr = mini_sh.ls_start;
			while(ptr)
			{printf("*type %d\n",ptr->type);
			int i=0;
			while(ptr->content[i])
			printf("%s\n",ptr->content[i++]);

			ptr=ptr->next;
			printf("\n");}
}