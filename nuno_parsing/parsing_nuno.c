/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_nuno.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/27 18:36:31 by tisantos          #+#    #+#             */
/*   Updated: 2021/04/27 20:09:14 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../get_next_line/get_next_line.h"
# include "../libft/libft.h"

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <string.h>

# define SHELL_DELIMITERS " \t\r\n\a"

# define STDIN 0
# define STDOUT 1
# define STDERR 2


/**************************************************************/
/*						Structs Stuff						  */
/**************************************************************/

typedef struct s_splvariab
{
	char			**tab;
	char			*next_str;
	unsigned int	next_str_len;
	unsigned int	nb_strs;
	unsigned int	i;
	int 			single_q;
	int 			double_q;

}						t_splvariab;
typedef struct s_linklis
{
	char				*pre_split;
	char				**content;
	int					type;
	struct s_linklis	*next;
}						t_linklis;
typedef struct	s_var_seplink
{
	int		i;
	int		s;
	int		single_q;
	int		double_q;
	char	n;
}						t_var_seplink;
typedef struct s_var_add_tlis
{
		int		i;
	t_list	*temp;
	char	*aux;
	int		start;
	int		last;
	int		type;
}						t_var_add_tlis;
typedef struct s_minishell
{
	t_linklis 		*ls_start;
	char			*line; // Line you write on your stdin.
	char			**cmd_tables; // Command tables where you store your commands separated by ;
	char			**args; // Line divided in arguments.
	char			**env; // Env file.
	int				status; // Status of while loop.
	int				absolute_path; // 1 to change to absolute, 0 for minishell 	>.
	int				testing; // Changes subject commands for real builtin commands.
	char			**history; // Saves the history of commands you typed.
	int				history_len; // How many commands you've typed.
	pid_t			pid;
	int 			error;
}					t_minishell;

t_minishell mini_sh;


/**************************************************************/
/*						Funções do Nuno						  */
/**************************************************************/

/* Utils */

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

/* split_ignore_inquot */

char	**ft_clsplit(char **tab)
{
	unsigned int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (NULL);
}
int che_open_quo(t_splvariab * varia, char t)
{
	if (t == '\"' && varia->double_q == 0 && varia->single_q == 0 )
		varia->double_q = 1;
	else if (t == '\"' && varia->double_q == 1 && varia->single_q == 0 )
		varia->double_q = 0;
	else if (t == '\'' && varia->single_q == 0  && varia->double_q == 0)
		varia->single_q = 1;
	else if (t == '\'' && varia->single_q == 1 && varia->double_q == 0 )
		varia->single_q = 0;
	if (varia->double_q == 0 && varia->single_q == 0)
		return (1);
	else
		return (0);
}
static unsigned int	ft_get_splnb(char const *s,
	const char *delimiters, t_splvariab *variab)
{
	unsigned int	i;
	unsigned int	nb_strs;

	if (!s[0])
		return (0);
	i = 0;
	nb_strs = 0;
	while (s[i] && ft_strchr(delimiters, s[i]))
		i++;
	while (s[i])
	{
		if (che_open_quo(variab, s[i]) && ft_strchr(delimiters, s[i]))
		{
			nb_strs++;
			while (s[i] && ft_strchr(delimiters, s[i]))
				i++;
			continue ;
		}
		else if(che_open_quo(variab, s[i]) && chck_iespac(s[i + 1], s[i]) )
			nb_strs++;
		i++;
	}
	if (!ft_strchr(delimiters, s[i - 1]))
		nb_strs++;
	return (nb_strs);
}
static void	ft_get_next_str1(char **next_str, unsigned int *next_str_len,
					const char *delimiters, t_splvariab *varia)
{
	unsigned int	i;
	char			k;
	int				a;
	int				o;

	*next_str += *next_str_len;
	*next_str_len = 0;
	i = 0;
	o = *next_str_len;
	while (**next_str && ft_strchr(delimiters, **next_str))
		(*next_str)++;
	while ((*next_str)[i])
	{
		k = (*next_str)[i];
		a=che_open_quo(varia, k);
		if (a && i!=o && (ft_strchr(delimiters,
			(*next_str)[i]) || chck_iespac((*next_str)[i+1], k) ))
			return ;
		else if(!a && i>0 && i!=o && chck_iespac((*next_str)[i-1],k))
			return;
		(*next_str_len)++;
		i++;

	}
}
char	**ft_split_igquo(char const *s, const char *delimiters)
{
	t_splvariab	var;

	var.double_q = 0;
	var.single_q = 0;
	var.nb_strs = ft_get_splnb(s, delimiters, &var);
	var.tab = malloc(sizeof(char *) * (var.nb_strs + 1));
	if (var.tab == NULL)
		return (NULL);
	var.i = 0;
	var.next_str = (char *)s;
	var.next_str_len = 0;
	while (var.i < var.nb_strs)
	{
		ft_get_next_str1(&var.next_str, &var.next_str_len, delimiters, &var);
		var.tab[var.i] = malloc(sizeof(char) * (var.next_str_len + 1));
		if (var.tab[var.i] == NULL)
			return (ft_clsplit(var.tab));
		ft_strlcpy(var.tab[var.i], var.next_str, var.next_str_len + 1);
		var.i++;
	}
	var.tab[var.i] = NULL;
	return (var.tab);
}


/* list_func2 */

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

/* list_func */

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
	if(va.double_q || va.single_q)
		mini_sh.error=1;
	return (0);
}
int	add_to_list(int index)
{
	t_var_add_tlis	va;

	add_var_init(&va);
	while (mini_sh.cmd_tables[index][va.i])
	{
		va.last = sep_link(index, &va.i, &va.type);
		printf("\\%d\\\n",va.last);
		if (va.last > 0 /*&& (va.last - va.start) > 1*/)
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
	
	//chck_dup_symbols();
		


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



/**************************************************************/
/*							 UTILS 							  */
/**************************************************************/

/* Sets the CMD List */

void	set_cmds()
{
	int i = 0;

	mini_sh.cmd_tables = malloc(sizeof(char *) * 2);

	mini_sh.cmd_tables[1] = NULL;
}


/**************************************************************/
/*							Main Stuff						  */
/**************************************************************/

int main ()
{
	int i = 0;
	set_cmds();

	while(i < 500)
	{
		mini_sh.line = NULL;
		get_next_line(0, &mini_sh.line);
		mini_sh.cmd_tables[0] = ft_strdup(mini_sh.line);
		//**********************************************//



		add_to_list(0);
		ft_linklstclear(&mini_sh.ls_start);



		//**********************************************//
		if (ft_strcmp(mini_sh.line, "exit") == 0)
		{
			free(mini_sh.line);
			free_array(mini_sh.cmd_tables);
			break;
		}
		free(mini_sh.line);
		free(mini_sh.cmd_tables[0]);
	}
}
