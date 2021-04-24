#include "../../shell.h"

/*t_linklis	*ft_linknew(char **content,int type)
{
	t_linklis	*new;
	int	i;
	int no_args;
	
	i = 0;
	new = malloc(sizeof(t_list));
	if (new == NULL)
		return (NULL);
	if (new)
	{
		while(content[i])
		new->content[i] = content;
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
*/

int is_separator(char check)
{
	if(check == '|')
	return 1;
	else 
	return 0;
}
int	sep_link(int index,int *a)
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
		else if (is_separator(mini_sh.cmd_tables[index][i]) && single_q == 0 && double_q == 0)
		{	//printf("--%d -%d-%d\n",double_q,single_q,i);
			return (i);
		}
		//printf("--%d -%d-%d\n",double_q,single_q,i);
		i++;
	}
	return(0);

}
int add_to_list(int index)
{
	int i;
	t_list *temp;
	int  start;
	int last;
	int open_quote;
	i = 0;
	open_quote = 0;
	start = 0;
	last =0;
	
	while(mini_sh.cmd_tables[index][i])
	{
		last=sep_link(index,&i);
		
		if (last > 0 && (last-start)>1)
		{
			printf("*%s*\n",ft_substr(mini_sh.cmd_tables[index],start,last-start));
			start = last + 1;
		}
		if(!mini_sh.cmd_tables[index][i+1])
		printf("*%s*\n",ft_substr(mini_sh.cmd_tables[index],start,(i+1)-start));
		//if(mini_sh.cmd_tables[index][i] == '\"' && open_quote)
	
		i++;
	}
	//ft_lstnew()
	//	ft_lstadd_back(mini_sh.start,)
}

