#include "../../shell.h"
char    **re_alloc_parse(char **original)
{
    int elems;
    int i;
    int k;
    char **ret;
    
    i = 0;
    elems = 0;
    if(!original)
    return NULL;
    while(original[i] && ft_strlen(original[i]))
        i++;
    ret = malloc(sizeof(char*)*(i+1));
    if (!ret)
        return NULL;
    elems = i;
    i = 0; 
    k = 0;   
    while(i < elems)
    {
        while(!ft_strlen(original[k]))
            k++;
        ret[i] = ft_strdup(original[k]);
        i++;
        k++;
    }
        ret[i] = NULL;
        free_array(original);
        return ret;
    
}

int check_builtinlst(char *str)
{
    if(ft_strcmp(str,"echo") == 0)
		return(1);
	else if(ft_strcmp(str,"cd") == 0)
		return(1);
	else if(ft_strcmp(str,"pwd") == 0)
		return(1);
	else if(ft_strcmp(str,"export") == 0)
		return(1);
	else if(ft_strcmp(str,"unset") == 0)
		return(1);
	else if(ft_strcmp(str,"env") == 0)
		return(1);
	else if(ft_strcmp(str,"exit") == 0)
		return(1);
	else if(ft_strcmp(str,"absolute_path") == 0)
		return(1);
	else if(ft_strcmp(str,"testing") == 0)
		return(1);
	else if(ft_strcmp(str,"history") == 0)
		return(1);
	else
		return (0);
}

void	ft_lstbuiltcheck(void)
{
	t_linklis	*lst;

	lst = mini_sh.ls_start;
	while (lst)
	{
		if(check_builtinlst(lst->content[0]))
            lst->builtin = 1;
        else
            lst->builtin = 0;
		lst = lst->next;
	}
}

void    ft_lstclear_zerolen(void)

{
	t_linklis	*lst;
    int i;

    i = 0;
	lst = mini_sh.ls_start;
	while (lst)
	{
		lst->content = re_alloc_parse(lst->content);        
		lst = lst->next;
	}
}