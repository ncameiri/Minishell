/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/02 15:39:03 by tisantos          #+#    #+#             */
/*   Updated: 2021/05/04 13:02:08 by tisantos         ###   ########.fr       */
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
	int					builtin;
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
static unsigned int	ft_get_splnb(char const *s,	const char *delimiters, t_splvariab *variab)
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
static void	ft_get_next_str1(char **next_str, unsigned int *next_str_len, const char *delimiters, t_splvariab *varia)
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
		//printf("||%s||",lst)
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
	ft_lsttrim();
	ft_lstclear_zerolen();
	ft_lstbuiltcheck();


	if(mini_sh.error==1)
	{
		ft_linklstclear(&mini_sh.ls_start);
		printf("bash: syntax error near unexpected token `||'\n");
		return (0);
	}
	else
		return (1);
}



/**************************************************************/
/*							Execute							  */
/**************************************************************/

// Type 1 = |
// Type 2 = >>
// Type 3 = >
// Type 4 = <
// Type 8 = Ultimo Elemento ou Vazio

/* Complicated Execute */

char	*get_outfile(t_linklis *list, int count)
{
	return (list->next->content[0]);
}

int	is_outfile(t_linklis *list, int count)
{
	int i;

	i = 0;

	while (i < count)
	{
		list->next;
		i++;
	}

	if (list->type == 3)
		return (1);
	return (0);
}

int	number_of_commands(t_linklis *list)
{
	int i;

	i = 0;
	while (list != NULL)
	{
		i++;
		list = list->next;
	}
	return (i);
}

char	*get_infile_last_arg(t_linklis *list)
{
	int	i;

	i = 0;
	while (list != NULL)
	{
		if (list->type == 4)
		{
			list = list->next;
			break;
		}
		list = list->next;
	}
	while (list->content[i] != NULL)
		i++;
	return (list->content[i - 1]);
}

int	has_infile(t_linklis *list)
{
	while (list != NULL)
	{
		if (list->type == 4)
			return (1);
		list = list->next;
	}
	return (0);
}

void	complicated_execute(t_linklis *list) // Teste
{
	t_linklis *start_list;

	start_list = list;
	list = list->next;

	// Save input and output //

	int tmpin;
	int tmpout;

	tmpin = dup(STDIN_FILENO);	 // 3	// Save stdin and stdout because you're going to change
	tmpout = dup(STDOUT_FILENO); // 4	// their values and you want to recover them in the end.

	// Loop all the commands seperated by | > >> < //

	int ret;
	int i;
	int fd[2];
	int num_commands;

	num_commands = number_of_commands(start_list);
	i = 0;

	while (i < num_commands)
	{
		if (start_list->type == 1) // If it's pipe.
		{
			if (pipe(fd) == -1)
				return ;


		}

		// Create a child process //

		ret = fork();
		if (ret == 0)
		{
			execvp(start_list->content[0], start_list->content);
			exit(1);
		}

		start_list = start_list->next;
		i++;
	}

}

/*void	complicated_execute(t_linklis *list)
{
	t_linklis *start_list;

	start_list = list;

	// Save input and output //

	int tmpin;
	int tmpout;

	tmpin = dup(STDIN_FILENO);	 // 3	// Save stdin and stdout because you're going to change
	tmpout = dup(STDOUT_FILENO); // 4	// their values and you want to recover them in the end.

	// Set the initial input //

	int fdin;

	fdin = 0;
	if (has_infile(list) == 1) // Use inline input
		fdin = open(get_infile_last_arg(list), O_RDONLY);
	else // Use default input
		fdin = dup(tmpin);

	// Loop all the commands seperated by | > >> < //

	int ret;
	int fdout;
	int i;
	int num_commands;

	num_commands = number_of_commands(list);
	i = 0;

	while (i < num_commands)
	{
		// Redirect input //
		dup2(fdin, 0);
		close(fdin);

		// Setup output //
		if (i == num_commands - 1) // Last simple command
		{
			if (is_outfile(start_list, i) == 1)
				fdout = open(get_outfile(start_list, i), O_WRONLY | O_TRUNC | O_CREAT, 0644);
			else // Use default output
				fdout = dup(tmpout);
		}
		else // Not Last Command
		{
			int fdpipe[2];
			pipe(fdpipe);
			fdout = fdpipe[1];
			fdin = fdpipe[0];
		}

		// Redirect output //

		dup2(fdout,1);
		close(fdout);

		// Create a child process //

		ret = fork();
		if (ret == 0)
		{
			execvp(list->content[0], list->content);
			exit(1);
		}

		list = list->next;
		i++;
	}

	// Restore in/out defaults //

	dup2(tmpin, 0);
	dup2(tmpout, 1);
	close(tmpin);
	close(tmpout);

	waitpid(ret, NULL, 0);




}
*/

/* Simple Execute */

void	run_bin_simple(t_linklis *list)
{
	char	*bin_path;

	bin_path = ft_strjoin("/usr/bin/", list->content[0]);

	mini_sh.pid = fork();

	if(mini_sh.pid == 0)
	{
		if (ft_strchr(list->content[0], '/'))
			execve(list->content[0], list->content, mini_sh.env); // This one is in case it's to read an ./a.out or other executable.
		execve(bin_path, list->content, mini_sh.env); // If it's not an executable, you read from /usr/bin/
		printf("%s: command not found\n", list->content[0]);
		free(bin_path);
		ft_linklstclear(&mini_sh.ls_start);
		exit(0);
	}
	else if (mini_sh.pid > 0)
		wait(&mini_sh.pid);
	else if (mini_sh.pid < 0)
		write(1,"ERROR",5);

	mini_sh.pid = 0;

	free(bin_path);
}

/* Main Function */

void	exec_command()
{
	t_linklis *list;

	list = mini_sh.ls_start;

	if (list->content[0] == NULL)
	{
		printf(" : command not found.\n");
		return ;
	}

	if (list->type == 8)
		run_bin_simple(list);
	else
		complicated_execute(list);
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
	int debuga = 1;
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

		exec_command();
		printf("\n");

		ft_linklstclear(&mini_sh.ls_start);



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
