#include "../../shell.h"
int quotes_check_1(char t, int *single_q, int *double_q)
{
	if (t == '\"' && *double_q == 0 && *single_q == 0 )
		*double_q = 1;
	else if (t == '\"' && *double_q == 1 && *single_q == 0 )
		*double_q = 0;
	else if (t == '\'' && *single_q == 0  && *double_q == 0)
		*single_q = 1;
	else if (t == '\'' && *single_q == 1 && *double_q == 0 )
		*single_q = 0;
	if (*double_q == 0 && *single_q == 0)
		return (1);
	else
		return (0);
}


void found_env(char ***original)
{
	int i;
	int k;
	int l;
	int old_index;
	char **ret;
	char *search;
	char temp[1001];
	i = -1;
	ret = *original;
	
	if(!(ret))
	return ;
	while(ret[++i])
	{
		if(ret [i][0] == '\'')
			continue;
		k = 0;
		l = 0;
		while(ret[i][k])
		{			
			if(ret[i][k] == '$' && ret[i][k+1] && !ft_strchr(SHELL_DELIMITERS,ret[i][k + 1]))
			{
				old_index = k;
				while(!ft_strchr(SHELL_DELIMITERS,ret[i][k + 1]) && ret[i][k+1])
				{
					
					if(ret[i][k+1] == '\"')
					break;
					k++;
				}
				search = ft_substr(ret[i], old_index + 1, k - old_index);
				if(ft_strlen(env_isex_elem(search)))
				{	
					ft_strlcpy(temp + l, env_isex_elem(search), ft_strlen(env_isex_elem(search)-1));
					l+=ft_strlen(env_isex_elem(search));
				}
				else
					k=old_index + k + 1;
				free(search);
			
			}
			else
			{
				temp[l] = ret[i][k];
				l++;
			}
			k++;
		}
		temp[l] = '\0';
		if ( l == 0)
		{	free(ret[i]);
			ret[i]= ft_strdup("");
		}
		if(l > 0)
		{
			free(ret[i]);
			ret[i] = ft_strdup(temp);
		}		
	}
}


int env_add_elem (char *set, char *content)
{//VERIFICAR SE JA EXISTE O ELEMENTO
	char *new_elem;
	new_elem = ft_strjoin(set,content);
	add_str_to_arrarr(mini_sh.env, new_elem);
	return 0;
}

int env_rm_elem (char *set)
{
	int a;
	int i;
	int k;
	char **temp;
	char *set_equal;

	i = 0;
	a = 0;
	k = 0;
	set_equal = ft_strjoin(set, "=");
	while (mini_sh.env[i] != NULL)
		i++;
	temp = malloc(sizeof(char *) * i);
	if (temp == NULL)
		return (-1);
	while(a < (i-1))
	{
		if(ft_strncmp(set_equal,mini_sh.env[k],ft_strlen(set_equal)))
		{
			temp[a] = ft_strdup(mini_sh.env[k]);
			a++;
		}
		k++;
	}
	//temp[a++] = ft_strdup(string);
	temp[a] = NULL;
	free(set_equal);
	free_array(mini_sh.env);
	mini_sh.env = temp;
	return (0);
}



	
int env_list_upd_elem ()
{
	t_linklis	*lst;

	lst = mini_sh.ls_start;
	while (lst)
	{
		found_env(&(lst->content));
		lst = lst->next;
	}
	return 0;
}

char *env_isex_elem (char *set)
{
	//RETORNA "" PARA NAO DAR SEGFAULT SE FT_STRLEN FOR 0 e sinal que nao existe
	int i;
	int k;
	char *set_equal;
	
	set_equal = ft_strjoin(set, "=");
	i = 0;
	k = ft_strlen(set_equal);
	while(mini_sh.env[i] != NULL)
	{
		if(!strncmp(set_equal,mini_sh.env[i],k))
		{
			free(set_equal);
			return(mini_sh.env[i] + k );
		}		
		i++;
	}
	ft_strlen(set_equal);
	free(set_equal);
	return("");
	/* char *ol;
    ol=env_isex_elem(mini_sh.ls_start->content[1]);
    if(ft_strlen(ol))
    printf("%s\n",ol);
    else 
    printf("DEU BOSTA\n");*/
}
int env_var_update(char *set_tbc, char *new_ct, int is_env)
{
	  char *elem;
    char *set;
    env_rm_elem(set_tbc);
	set = ft_strjoin(set_tbc,"=");
	if(is_env)
		elem = ft_strjoin(set, env_isex_elem(new_ct));
    else
		elem = ft_strjoin(set,new_ct);
	mini_sh.env=add_str_to_arrarr(mini_sh.env,elem);
	free(set);
	free(elem);
	return 0;
}
