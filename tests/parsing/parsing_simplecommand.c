/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_simplecommand.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/02 15:39:03 by tisantos          #+#    #+#             */
/*   Updated: 2021/05/07 04:22:25 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../get_next_line/get_next_line.h"
# include "../../libft/libft.h"

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <string.h>

# define SHELL_DELIMITERS " \t\r\n\a"
# define DELIMETERS2 " \t\r\n\a\'\""

# define STDIN 0
# define STDOUT 1
# define STDERR 2


/**************************************************************/
/*						Structs Stuff						  */
/**************************************************************/

typedef struct s_simplecommand_temp		// <---- Adicionar este.
{
	char		**temp_command;
	char		**temp_infile;
	char		**temp_outfile;
	int			temp_builtin;

	int			temp_outfiles;
	int			temp_infiles;

	int			temp_append;

	char		*temp_outfile_extra_text;
	char		*temp_infile_extra_text;

}		t_simplecommand_temp;

typedef struct s_simplecommand			// <---- Adicionar este.
{
	char					**command;
	char					**outfile;	// These are all outfiles.
	char					**infile;	// These are all infile.
	int						builtin;

	int						outfiles;
	int						infiles;
	int						append; // Se é para fazer append ou um outfile normal.

	char					*outfile_extra_text; // Caso "echo ola > aqui.txt texto aqui > alo.txt"
	char					*infile_extra_text; // Caso "echo ola < aqui.txt texto aqui < alo.txt"

	struct s_simplecommand	*next;

}				t_simplecommand;

typedef struct s_splvariab
{
	char			**tab;
	char			*next_str;
	unsigned int	next_str_len;
	unsigned int	nb_strs;
	unsigned int	i;
	int 			single_q;
	int 			double_q;

}				t_splvariab;
typedef struct s_linklis
{
	char				*pre_split;
	char				**content;
	int					type;
	int					builtin;
	struct s_linklis	*next;
}				t_linklis;
typedef struct	s_var_seplink
{
	int		i;
	int		s;
	int		single_q;
	int		double_q;
	char	n;
}				t_var_seplink;
typedef struct s_var_add_tlis
{
	int		i;
	t_list	*temp;
	char	*aux;
	int		start;
	int		last;
	int		type;
}			t_var_add_tlis;

typedef struct s_minishell
{
	t_simplecommand	*simple_cmd; // <<<--- -Adicionar este

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
	char			*error_log;
	int				actind;
}					t_minishell;

t_minishell mini_sh;


/**************************************************************/
/*						Funções do Nuno						  */
/**************************************************************/

/* utils */

int	only_spaces(char *line)
{
	int i;
	int a;

	i = 0;
	a = 0;
	while (line[i] != '\0')
	{
		if (line[i] != ' ')
			a++;
		i++;
	}
	if (a > 0)
		return (0);
	else
		return (1);
}
int	*add_int_to_arr(int *array, int location, int count)
{
	int a;
	int i;
	int *temp;

	i = 0;
	a = 0;

	while (i < count)
		i++;
	temp = malloc(sizeof(int) * (i + 2));
	if (temp == NULL)
		return (NULL);
	while(a < i)
	{
		temp[a] = array[a];
		a++;
	}
	temp[a++] = location;
	temp[a] = '\0';

	free(array);

	return (temp);
}
char	**add_str_to_arrarr(char **array, char *string)
{
	int a;
	int i;
	char **temp;

	i = 0;
	a = 0;
	while (array[i] != NULL)
		i++;
	temp = malloc(sizeof(char *) * (i + 2));
	if (temp == NULL)
		return (NULL);
	while(a < i)
	{
		temp[a] = ft_strdup(array[a]);
		a++;
	}
	temp[a++] = ft_strdup(string);
	temp[a] = NULL;
	free_array(array);
	return (temp);
}
void debug(int a)
{
	t_linklis *ptr;

	int i = 0;
	printf("\n------------COMMAND TABLE [%i]------------\n", a);
	printf("\n%s\n", mini_sh.cmd_tables[a]);
	ptr = mini_sh.ls_start;
	while(ptr)
	{
		printf("\n");
		printf("[ %s]\n", ptr->pre_split);


		if (ptr->type == 1)
			printf("Type %d = | \n", ptr->type);
		else if (ptr->type == 2)
			printf("Type %d = >> \n", ptr->type);
		else if (ptr->type == 3)
			printf("Type %d = > \n", ptr->type);
		else if (ptr->type == 4)
			printf("Type %d = < \n", ptr->type);
		else if (ptr->type == 8)
			printf("Type %d = last arg \n", ptr->type);

		printf("Builtin: %i\n", ptr->builtin);

		while(ptr->content[i] != NULL)
		{
			printf("arg[%i] = %s\n", i, ptr->content[i]);
			i++;
		}
		printf("arg[%i] = %s\n", i, ptr->content[i]);

		i = 0;
		ptr=ptr->next;
	}
}
char	*ft_errstr(char c)
{
	char *ret;

	ret = malloc(2);
	if (!ret)
		return NULL;
	else
	{
		ret[0] = c;
		ret[1] = 0;
		return ret;
	}
}

/* add_to_list 5*/

char	**re_alloc_parse(char **original)
{
	int elems;
	int i;
	int k;
	char **ret;

	i = 0;
	elems = 0;
	if(!original)
	return NULL;
	while(original[i]/* && ft_strlen(original[i])*/)
	{
		if(ft_strlen(original[i]))
		elems++;
		i++;
	}
	ret = malloc(sizeof(char*)*(elems+1));
	if (!ret)
		return NULL;

	i = 0;
	k = 0;
	while(i < elems)
	{
		while(!ft_strlen(original[k]) && original[k])
			k++;
		if(original[k])
		ret[i] = ft_strdup(original[k]);
		i++;
		k++;
	}
	ret[i] = NULL;
	free_array(original);
	return (ret);

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
		if(lst->content[0] && check_builtinlst(lst->content[0]))
			lst->builtin = 1;
		else
			lst->builtin = 0;
		lst = lst->next;
	}
}
void	ft_lstclear_zerolen(void)
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

/* add_to_list 4*/

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
				{
					if(!mini_sh.error_log)
					mini_sh.error_log = ft_strjoin("",lst->content[i]);
					mini_sh.error = 1;
				}
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
	char		*old;


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
			{if(!mini_sh.error_log)
					mini_sh.error_log = ft_errstr(pt[i]);
				mini_sh.error = 1;}
			i = ft_strlen(pt) - 1;
			while (pt[i] && i>0 && ft_strchr(SHELL_DELIMITERS,pt[i]))
				i--;
			if (ft_strchr("|><", pt[i]))
				{if(!mini_sh.error_log)
					mini_sh.error_log = ft_errstr(pt[i]);
					mini_sh.error = 1;}
		}
		else
		{	if (!mini_sh.error_log)
			mini_sh.error_log = ft_strdup(mini_sh.cmd_tables[mini_sh.actind]+ft_strlen(old));
			mini_sh.error =1;
		}
		lst = lst->next;
		old = pt;
	}
}

/* add_to_list 3*/

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
	int k;

	if (!s[0])
		return (0);
	i = 0;
	nb_strs = 0;
	while (s[i] && ft_strchr(delimiters, s[i]))
		i++;
	while (s[i])
	{

		k=che_open_quo(variab, s[i]);
		if (ft_strchr(delimiters, s[i]) && k)
		{
			nb_strs++;
			while (s[i] && ft_strchr(delimiters, s[i]))
				i++;
			continue ;
		}
		/*else if(k && chck_iespac(s[i + 1], s[i]) )
			{nb_strs++;}
		else if(i>0 && k && chck_iespac(s[i],s[i - 1]) )
			{nb_strs++;}*/
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
	//o = *next_str_len;
	while (**next_str && ft_strchr(delimiters, **next_str))
		(*next_str)++;
	o=0;
	while ((*next_str)[i])
	{
		k = (*next_str)[i];
		a=che_open_quo(varia, k);
		/*if(i>0)
		k = (*next_str)[i+1];
		else
		k = (*next_str)[i];

		if (chck_iespac((*next_str)[i],k) )
			{printf("**1%c**",k);
			return ;}
		(*next_str_len)++;
		i++;*/
			if (a && ft_strchr(delimiters, (*next_str)[i]))
			return ;
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
	var.single_q = 0;
	var.double_q = 0;
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

/* add_to_list 2*/

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
		lst->content = ft_split_igquo(lst->pre_split, SHELL_DELIMITERS);
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
	mini_sh.error_log = NULL;
	mini_sh.actind = 0;
}

/* add_to_list */

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
			return (va.i /*+(va.n == '>' && mini_sh.cmd_tables[index][va.i + 1] == '>')*/);
		va.i++;
		va.n = mini_sh.cmd_tables[index][va.i];
	}
	if(va.double_q || va.single_q)
		{mini_sh.error=1;}
	return (0);
}
int	add_to_list(int index)
{
	t_var_add_tlis	va;

	add_var_init(&va);
	mini_sh.actind = index;
	while (mini_sh.cmd_tables[index][va.i])
	{
		va.last = sep_link(index, &va.i, &va.type);
		if (va.last > 0 /*&& (va.last - va.start) > 1*/)
		{
			va.aux = ft_substr(mini_sh.cmd_tables[index],
					va.start, va.last - va.start);
			ft_linkadd_back(&mini_sh.ls_start, ft_linknew(va.aux, va.type));
			va.start = va.last + 1;
			if (va.type == 2)
			{
				va.start += 1;
				va.i++;
			}

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
	//ft_lsttrim();
	ft_lstclear_zerolen();
	ft_lstbuiltcheck();


	if(mini_sh.error==1)
	{
		ft_linklstclear(&mini_sh.ls_start);
		if(ft_strlen(mini_sh.error_log) > 1)
			printf("bash: syntax error near unexpected token `%.2s\'\n",mini_sh.error_log);
		else
			printf("bash: syntax error near unexpected token `%s'\n",mini_sh.error_log);
		free (mini_sh.error_log);
		return (0);
	}
	else
		return (1);
}


/**************************************************************/
/*						Simple Commands						  */
/**************************************************************/

// Type 1 = |
// Type 2 = >>
// Type 3 = >
// Type 4 = <
// Type 8 = Ultimo Elemento ou Vazio


/* Utils */

void	debug_simple_commands()
{
	// DEBUG //

	int i;
	int a;

	a = 0;

	t_simplecommand *temp;

	temp = mini_sh.simple_cmd;

	while (temp != NULL)
	{
		i = 0;
		printf("\n");
		printf("\n------------SIMPLE COMMAND[%i]------------\n", a);
		while (temp->command[i] != NULL)
		{
			printf("Command[%i] = %s \n",i, temp->command[i]);
			i++;
		}
		printf("Command[%i] = %s \n",i, temp->command[i]);
		if (i == 0)
			printf("Command = (null)\n");
		printf("Builtin = %i \n", temp->builtin);
		i = 0;
		while (temp->outfile[i] != NULL)
		{
			printf("Outfile[%i] = %s \n", i, temp->outfile[i]);
			i++;
		}
		if (i == 0)
			printf("Outfile[%i] = %s \n",i ,temp->outfile[i]);
		else
			printf("Outfile[%i] = %s \n", i, temp->outfile[i]);
		printf("Outfiles = %i \n", temp->outfiles);
		printf("Outfile extra text = %s\n", temp->outfile_extra_text);
		printf("Append = %i \n", temp->append);

		i = 0;
		while (temp->infile[i] != NULL)
		{
			printf("Infile[%i] = %s \n", i, temp->infile[i]);
			i++;
		}
		if (i == 0)
			printf("Infile[%i] = %s \n",i, temp->infile[i]);
		else
			printf("Infile[%i] = %s \n",i, temp->infile[i]);

		printf("Infiles = %i \n", temp->infiles);
		printf("Infile extra text = %s\n", temp->infile_extra_text);

		temp = temp->next;
		a++;
	}
}
void	ft_lstclear_simple_struct(t_simplecommand **lst)
{
	t_simplecommand	*current;
	t_simplecommand	*next;

	if (!lst)
		return ;
	current = *lst;
	while (current != NULL)
	{
		next = current->next;
		free_array(current->command);
		free_array(current->infile);
		free_array(current->outfile);
		free(current->outfile_extra_text);
		free(current->infile_extra_text);
		free(current);
		current = next;
	}
	*lst = NULL;
}
char	**new_array_array(char **args)
{
	int a;
	int i;
	char **temp;

	a = 0;
	i = 0;
	if (args == NULL)
	{
		temp = malloc(sizeof(char *) * 1);
		temp[0] = NULL;
		return (temp);
	}
	while (args[a] != NULL)
		a++;
	temp = malloc(sizeof(char *) * (a + 1));
	if (temp == NULL)
		return (NULL);
	while(args[i] != NULL)
	{
		temp[i] = ft_strdup(args[i]);
		i++;
	}
	temp[i] = NULL;
	return (temp);
}
char	*ft_strjoin_free(char *s1, char const *s2)
{
	char	*string;
	int		len;
	int		i;
	int		b;

	i = 0;
	b = 0;
	if (!s1)
		return (NULL);
	if (!s2 && s1)
		return ((char *)s1);
	len = strlen(s1) + strlen(s2);
	string = malloc(len + 1 * sizeof(string));
	if (string == NULL)
		return (NULL);
	while (s1[i] != '\0')
	{
		string[i] = s1[i];
		i++;
	}
	while (s2[b] != '\0')
		string[i++] = s2[b++];
	string[i] = '\0';
	free(s1);
	return (string);
}


void	*ft_lstnew_simple_add3(t_simplecommand_temp add)
{
	mini_sh.simple_cmd->next = malloc(sizeof(t_simplecommand));
	mini_sh.simple_cmd = mini_sh.simple_cmd->next;
	if (mini_sh.simple_cmd == NULL)
		return (NULL);
	if (mini_sh.simple_cmd)
	{
		mini_sh.simple_cmd->command = new_array_array(add.temp_command);
		mini_sh.simple_cmd->infile = new_array_array(add.temp_infile);
		mini_sh.simple_cmd->outfile = new_array_array(add.temp_outfile);
		mini_sh.simple_cmd->builtin = add.temp_builtin;
		mini_sh.simple_cmd->outfiles = add.temp_outfiles;
		mini_sh.simple_cmd->infiles = add.temp_infiles;
		mini_sh.simple_cmd->append = add.temp_append;
		if (add.temp_outfile_extra_text != NULL)
			mini_sh.simple_cmd->outfile_extra_text = ft_strdup(add.temp_outfile_extra_text);
		else
			mini_sh.simple_cmd->outfile_extra_text = NULL;
		if (add.temp_infile_extra_text != NULL)
				mini_sh.simple_cmd->infile_extra_text = ft_strdup(add.temp_infile_extra_text);
		else
			mini_sh.simple_cmd->infile_extra_text = NULL;
		mini_sh.simple_cmd->next = NULL;
	}
}
void	*ft_lstnew_simple_add2(t_simplecommand_temp add)
{
	t_simplecommand *start;
	t_simplecommand *temp;
	int i;

	start = mini_sh.simple_cmd;
	temp = mini_sh.simple_cmd;
	i = 0;
	while (temp != NULL)
	{
		i++;
		temp = temp->next;
	}
	while (i > 1)
	{
		mini_sh.simple_cmd = mini_sh.simple_cmd->next;
		i--;
	}
	ft_lstnew_simple_add3(add);
	mini_sh.simple_cmd = start;
}
void	*ft_lstnew_simple_add1(t_simplecommand_temp add)
{
	mini_sh.simple_cmd->command = new_array_array(add.temp_command);
	mini_sh.simple_cmd->infile = new_array_array(add.temp_infile);
	mini_sh.simple_cmd->outfile = new_array_array(add.temp_outfile);
	mini_sh.simple_cmd->builtin = add.temp_builtin;
	mini_sh.simple_cmd->outfiles = add.temp_outfiles;
	mini_sh.simple_cmd->infiles = add.temp_infiles;
	mini_sh.simple_cmd->append = add.temp_append;
	if (add.temp_outfile_extra_text != NULL)
		mini_sh.simple_cmd->outfile_extra_text = ft_strdup(add.temp_outfile_extra_text);
	else
		mini_sh.simple_cmd->outfile_extra_text = NULL;
	if (add.temp_infile_extra_text != NULL)
		mini_sh.simple_cmd->infile_extra_text = ft_strdup(add.temp_infile_extra_text);
	else
		mini_sh.simple_cmd->infile_extra_text = NULL;
	mini_sh.simple_cmd->next = NULL;
}
void	*ft_lstnew_simple_add(t_simplecommand_temp add)
{
	if (mini_sh.simple_cmd == NULL)
	{
		mini_sh.simple_cmd = malloc(sizeof(t_simplecommand));
		if (mini_sh.simple_cmd == NULL)
			return (NULL);
		if (mini_sh.simple_cmd)
		{
			ft_lstnew_simple_add1(add);
		}
	}
	else
		ft_lstnew_simple_add2(add);
}

/* Main Stuff */

int	iterations_in_simple_command(t_linklis *list)
{
	int	i;

	i = 0;
	while (list != NULL && list->type != 8 && list->type != 1)
	{
		i++;
		list = list->next;
	}
	if (i > 0)
		i++;
	return (i);

}

void *if_redirections_infile_2(t_linklis *list,
											t_simplecommand_temp *temp, int i)
{
	while (list->content[i] != NULL)
	{
		if (temp->temp_infile_extra_text == NULL)
			temp->temp_infile_extra_text = ft_strdup(list->content[i]);
		else
		{
			temp->temp_infile_extra_text = ft_strjoin_free(temp->temp_infile_extra_text,
													" ");
			temp->temp_infile_extra_text = ft_strjoin_free(temp->temp_infile_extra_text,
													list->content[i]);
		}
		i++;
	}
}

t_linklis *if_redirections_infile(t_linklis *list,
											t_simplecommand_temp *temp, int b)
{
	int i;
	int a;

	i = 0;
	a = 0;
	list = list->next;
	if (temp->temp_infile == NULL)
		temp->temp_infile = malloc(sizeof(char *) * 50);
	else
	{
		while (temp->temp_infile[a] != NULL)
			a++;
	}
	while (list->content[i] != NULL)
	{
		temp->temp_infile[a] = ft_strdup(list->content[i]);
		i++;
		a++;
		temp->temp_infiles++;
	}
	temp->temp_infile[a] = NULL;
	i = 1;
	if_redirections_infile_2(list, temp, i);
	return(list);
}

void	if_redirections_outfile_2(t_linklis *list,
											t_simplecommand_temp *temp, int i)
{
	while (list->content[i] != NULL)
	{
		if (temp->temp_outfile_extra_text == NULL)
			temp->temp_outfile_extra_text = ft_strdup(list->content[i]);
		else
		{
			temp->temp_outfile_extra_text = ft_strjoin_free(temp->temp_outfile_extra_text,
													" ");
			temp->temp_outfile_extra_text = ft_strjoin_free(temp->temp_outfile_extra_text,
													list->content[i]);
		}
		i++;
	}
}

t_linklis *if_redirections_outfile(t_linklis *list,
											t_simplecommand_temp *temp, int a)
{
	int i;

	i = 1;

	if (list->type == 3) // >
		temp->temp_append = 0;
	else if (list->type == 2) // >>
		temp->temp_append = 1;

	list = list->next;

	temp->temp_outfiles++;

	if (temp->temp_outfile == NULL)
		temp->temp_outfile = malloc(sizeof(char *) * 50);
	temp->temp_outfile[a] = ft_strdup(list->content[0]);
	temp->temp_outfile[a + 1] = NULL;

	if_redirections_outfile_2(list, temp, i);

	return (list);
}


void	if_redirections_3(t_simplecommand_temp temp)
{
	if (temp.temp_outfile != NULL)
		free_array(temp.temp_outfile);
	if (temp.temp_infile != NULL)
		free_array(temp.temp_infile);
	if (temp.temp_outfile_extra_text != NULL)
		free(temp.temp_outfile_extra_text);
	if (temp.temp_infile_extra_text != NULL)
		free(temp.temp_infile_extra_text);
}

t_simplecommand_temp if_redirections_2(t_simplecommand_temp temp, t_linklis *list)
{
	temp.temp_command = list->content;
	temp.temp_infile = NULL;
	temp.temp_outfile = NULL;
	temp.temp_builtin = list->builtin;
	temp.temp_outfiles = 0;
	temp.temp_infiles = 0;
	temp.temp_append = 0;
	temp.temp_outfile_extra_text = NULL;
	temp.temp_infile_extra_text = NULL;

	return (temp);
}

t_linklis  *if_redirections(t_linklis *list)
{
	t_simplecommand_temp temp;
	int iterations;
	int i;
	int a;
	int b;

	temp = if_redirections_2(temp, list);
	iterations = iterations_in_simple_command(list);
	i = 1;
	a = 0;
	b = 0;
	while(i < iterations)
	{
		if (list->type == 3 || list->type == 2) // > ou >> outfile
			list = if_redirections_outfile(list, &temp, a++);
		else if (list->type == 4) // < infile
			list = if_redirections_infile(list, &temp, b++);
		i++;
	}
	list = list->next;
	ft_lstnew_simple_add(temp);
	if_redirections_3(temp);
	return (list);
}

t_linklis	*if_no_redirections(t_linklis *list)
{
	t_simplecommand_temp temp;

	temp.temp_command = list->content;
	temp.temp_infile = NULL;
	temp.temp_outfile = NULL;
	temp.temp_builtin = list->builtin;
	temp.temp_outfiles = 0;
	temp.temp_infiles = 0;
	temp.temp_append = 0;
	temp.temp_outfile_extra_text = NULL;
	temp.temp_infile_extra_text = NULL;

	ft_lstnew_simple_add(temp);

	list = list->next;

	return (list);
}

void	add_to_simple_commands_list()
{
	t_linklis *list;
	list = mini_sh.ls_start;

	mini_sh.simple_cmd = NULL;

	while (list != NULL)
	{											// "echo ola" ou "echo ola > alo | echo ola"
		if (list->type == 1 || list->type == 8) // Se houver só um simple command.
		{										// Ou se for só 1 pipe sem redirections.
			list = if_no_redirections(list);
		}
		else
		{									// "echo ola > aqui bla < ali bla"
			list = if_redirections(list);	// Se tiver redirections.
		}
	}
}



/**************************************************************/
/*							 Utils							  */
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
	int debuga = 0;
	set_cmds();


	while(i < 500)
	{
		mini_sh.line = NULL;
		get_next_line(0, &mini_sh.line);
		mini_sh.cmd_tables[0] = ft_strdup(mini_sh.line);
		//**********************************************//

		add_to_list(0);
		if (debuga == 1)
			debug(i);

		add_to_simple_commands_list();

		debug_simple_commands();

		printf("\n");

		ft_linklstclear(&mini_sh.ls_start);
		ft_lstclear_simple_struct(&mini_sh.simple_cmd);


		//**********************************************//
		if (ft_strcmp(mini_sh.line, "exit") == 0)
		{
			free(mini_sh.line);
			free_array(mini_sh.cmd_tables);
			break;
		}
		if (ft_strcmp(mini_sh.line, "debug") == 0)
		{
			if (debuga == 0)
			{
				printf("debug = on\n");
				debuga = 1;
			}
			else if (debuga == 1)
			{
				printf("debug = off\n");
				debuga = 0;
			}
		}
		free(mini_sh.line);
		free(mini_sh.cmd_tables[0]);
	}
}
