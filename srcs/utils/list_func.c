#include "../../shell.h"

t_linklis	*ft_linknew(char *pre_split,int type)
{
	t_linklis	*new;
	int	i;
	int no_args;

	i = 0;
	new = malloc(sizeof(t_linklis));
	if (new == NULL)
		return (NULL);
	if (new)
	{
		new->pre_split=pre_split;
		new->type=type;
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
void ft_lsttrim()
{
	t_linklis *lst;
	int i;
	char *tmp;
	lst=mini_sh.ls_start;
	while (lst)
	{	i=0;
		while (lst->content[i])
		{
			tmp =ft_strtrim(lst->content[i],"\'\"");
			free(lst->content[i]);
			lst->content[i]=tmp;
			i++;
		}
		lst = lst->next;

	}

}

void	ft_lstspli()
{
	t_linklis *lst;
	lst=mini_sh.ls_start;
	while (lst)
	{
		lst->content=ft_split_igquo(lst->pre_split," ");
		lst = lst->next;
	}
}


int is_separator(char check,char check2, int *type)
{
	if(check == '|')
	{
		*type = 1;
		return 1;
	}
	else if (check == '>')
	{
		*type = 2;
		return 1;
	}
	else
	return 0;
}
int	sep_link(int index,int *a, int *type)
{
	int i;
	int s;
	int single_q;
	int double_q;

	i = *a;
	s = 0;
	single_q = 0;
	double_q = 0;
	while (mini_sh.cmd_tables[index][i] != '\0')
	{
		*a=i;
		if (mini_sh.cmd_tables[index][i] == '\"' && double_q == 0 && single_q == 0 )
			double_q = 1;
		else if (mini_sh.cmd_tables[index][i] == '\"' && double_q == 1 && single_q == 0 )
			double_q = 0;
		else if (mini_sh.cmd_tables[index][i] == '\'' && single_q == 0  && double_q == 0)
			single_q = 1;
		else if (mini_sh.cmd_tables[index][i] == '\'' && single_q == 1 && double_q == 0 )
			single_q = 0;
		else if (is_separator(mini_sh.cmd_tables[index][i],mini_sh.cmd_tables[index][i+1], type) && single_q == 0 && double_q == 0)
		{
			return (i);
		}

		i++;
	}
	return(0);

}
int add_to_list(int index)
{
	int i;
	t_list *temp;
	char *aux;
	int  start;
	int last;
	int type;
	i = 0;
	start = 0;
	last =0;
	temp = NULL;
	while(mini_sh.cmd_tables[index][i])
	{
		last=sep_link(index,&i,&type);

		if (last > 0 && (last-start)>1)
		{
			aux=ft_substr(mini_sh.cmd_tables[index],start,last-start);
			ft_linkadd_back(&mini_sh.ls_start,ft_linknew(aux,type));

			start = last + 1;
		}
		if(!mini_sh.cmd_tables[index][i+1])
		{
			type=8;
			aux=ft_substr(mini_sh.cmd_tables[index],start,(i+1)-start);
			ft_linkadd_back(&mini_sh.ls_start,ft_linknew(aux,type));
		}
		i++;
	}
/*
ISTO E PARA VER OS ELEM PRE_SPLIT
*/
/*	t_linklis *ptr;
			ptr = mini_sh.ls_start;
			while(ptr)
			{printf("*type %d*%s*\n",ptr->type,ptr->pre_split);

			ptr=ptr->next;}
//PARA CIMA E TEMPORARIO
ft_lstspli();//<--- IMPORTANTE
ft_lsttrim();
printf("ORGANIZADA\n");
			ptr = mini_sh.ls_start;
			while(ptr)
			{printf("*type %d\n",ptr->type);
			int i=0;
			while(ptr->content[i])
			printf("%s\n",ptr->content[i++]);

			ptr=ptr->next;
			printf("\n\n");}*/

}

