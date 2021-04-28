# include "../get_next_line/get_next_line.h"
# include "../libft/libft.h"

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <limits.h>
# include <errno.h>
# include <string.h>
# include <dirent.h>
# include <sys/wait.h>
# include <signal.h>
# include <termios.h>
# include <term.h>
# include <curses.h>
# include <termcap.h>
# include <sys/ioctl.h>
# include <sys/stat.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif
# define GNL_SUCCESS 1
# define GNL_EOF 0
# define GNL_ERROR -1

# define ERROR_NULL -7
# define ERROR_RDIR -6
# define ERROR_COLON2 -5
# define ERROR_PIPE2 -4
# define ERROR_COLON -3
# define ERROR_PIPE -2
# define ERROR_QUOTE -1
# define COMMON 0
# define SPACE 1
# define SINGLE 2
# define DOUBLE 3
# define LEFT 4
# define RIGHT 5
# define DOUBLERIGHT 6
# define BACKSLASH 7
# define PIPE 8
# define SEMICOLON 9
# define DOLLAR 10

# define ERROR 0

# define FALSE 0
# define TRUE 1

# define NORMAL_TYPE 0
# define PIPE_TYPE 8
# define COLON_TYPE 9

# define EXECVE_ERR 1
# define NOT_FOUND 2
# define IS_DIR 3
# define NO_F_OR_D 4


/**************************************************************/
/*						   Global Stuff						  */
/**************************************************************/

char **history;
int history_location;

typedef struct		s_path
{
	char			*path;
	struct s_path	*next;
}					t_path;
typedef struct		s_term
{
	struct termios	term;
	struct termios	save;
	char			*cm;
	char			*ce;
}					t_term;
typedef struct		s_cmd
{
	char			**av;
	int				ac;
	int				type;
	int				pip[2];
	int				fd_in;
	int				fd_out;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}					t_cmd;
typedef struct		s_env
{
	char			*key;
	char			*value;
	int				has_equal;
	struct s_env	*next;
}					t_env;
typedef struct		s_token
{
	char			*str;
	int				type;
	struct s_token	*next;
}					t_token;
typedef struct		s_keypos
{
	int				col;
	int				row;
}					t_keypos;
typedef struct		s_save
{
	char			*input;
	int				flag;
	struct s_save	*prev;
	struct s_save	*next;
}					t_save;
typedef struct		s_state
{
	t_token			*token_head;
	t_env			*env_head;
	t_cmd			*cmd_head;
	t_path			*path_head;
	t_term			t;
	t_save			*save_head;
	int				s_flag;
	char			*input;
	char			*input2;
	int				is_fork;
	int				ret;
	t_keypos		cur;
	t_keypos		max;
	t_keypos		start;
}					t_state;

t_state				g_state;




void	add_env_back(t_env **head, char *key, char *value, int has_equal);
void	clear_prompt(t_state *s);
void	move_cursor(t_state *s);
void	set_cursor(int *col, int *row);
char	*delete_last_char(char *str);


/**************************************************************/
/*							Signals							  */
/**************************************************************/

void	handle_signal(int signo)
{
	if (signo == SIGINT)
	{
		if (g_state.is_fork == 0)
		{
			write(1, "\nbash> ", 7);
			if (g_state.input)
				free(g_state.input);
			g_state.input = 0;
			g_state.ret = 1;
		}
		else
		{
			g_state.ret = 130;
			write(1, "\n", 1);
		}
	}
	else if (signo == SIGQUIT)
	{
		if (g_state.is_fork == 1)
		{
			g_state.ret = 131;
			write(1, "Quit: 3\n", 9);
		}
	}
}


/**************************************************************/
/*							 UTILS 							  */
/**************************************************************/

/* Functions */

int	ftt_calloc(size_t n, size_t size, void **ret)
{
	*ret = (char *)malloc(size * n);
	if (!*ret)
		return (0);
	ft_memset(*ret, 0, (n * size));
	return (1);
}
int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (s1 == 0)
		return (-1);
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	if (s1[i] == '\0' && s2[i] == '\0')
		return (0);
	return (s1[i] - s2[i]);
}
int		ft_putchar(int c)
{
	return (write(1, &c, 1));
}

/* Sets the History List */

void	set_history()
{
	history = malloc(sizeof(char *) * 8);
	history[0] = strdup("Um");
	history[1] = strdup("Dois");
	history[2] = strdup("Tres");
	history[3] = strdup("Quatro");
	history[4] = strdup("Cinco");
	history[5] = strdup("Cinco");
	history[6] = strdup("Seis");
	history[7] = NULL;

	history_location = 6;

	printf("History List is the following:\n\n");

	for (size_t i = 0; history[i] != NULL; i++)
	{
		printf("%s\n", history[i]);
	}

	printf("\n");
}

/**************************************************************/
/*						 Parse Env List						  */
/**************************************************************/

/* Parse Env List */

char	*ft_strjoin2(char *s1, char *s2)
{
	char	*res;
	size_t	len;
	int		i;

	if (!(s1) && !(s2))
		return (0);
	else if (!(s1) || !(s2))
	{
		if (!(s1))
			return (ft_strdup(s2));
		else
			return (ft_strdup(s1));
	}
	len = (ft_strlen(s1)) + (ft_strlen(s2));
	if (!ftt_calloc(sizeof(char), len + 1, (void *)&res))
		exit(1);
	i = 0;
	while (*s1)
		res[i++] = *s1++;
	while (*s2)
		res[i++] = *s2++;
	res[i] = '\0';
	return (res);
}
char	*changed_str(char *origin, int start, int end, char *insert)
{
	char	*result;
	char	*front;
	char	*front_insert;
	char	*back;

	front = ft_substr(origin, 0, start);
	front_insert = ft_strjoin2(front, insert);
	back = ft_substr(origin, end + 1, ft_strlen(origin) - (end - start));
	result = ft_strjoin2(front_insert, back);
	free(front);
	free(back);
	free(insert);
	free(front_insert);
	return (result);
}
int		is_operator(char *c, int i)
{
	if (c[i] == '\'')
		return (SINGLE);
	else if (c[i] == '\"')
		return (DOUBLE);
	else if (c[i] == '<')
		return (LEFT);
	else if (c[i] == '>')
	{
		if (c[i + 1] == '>')
			return (DOUBLERIGHT);
		return (RIGHT);
	}
	else if (c[i] == ';')
		return (SEMICOLON);
	else if (c[i] == '|')
		return (PIPE);
	else if (c[i] == ' ')
		return (SPACE);
	else if (c[i] == '\\')
		return (BACKSLASH);
	else if (c[i] == '$')
		return (DOLLAR);
	return (COMMON);
}
int		check_key_len(char *str, int is_tokenizer)
{
	int len;

	len = 0;
	while (str[len])
	{
		if (str[len] == '\'' || str[len] == ' ')
			break ;
		if (is_operator(str, len) && is_tokenizer)
			break ;
		len++;
	}
	return (len);
}
void	print_env_all(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (tmp->has_equal == TRUE)
		{
			if (tmp->value)
				printf("%s=%s\n", tmp->key, tmp->value);
			else
				printf("%s=\n", tmp->key);
		}
		tmp = tmp->next;
	}
}
t_env	*find_env(t_env *head, char *key)
{
	t_env *env;

	env = head;
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (env);
		env = env->next;
	}
	return (NULL);
}
char	*find_env_val(t_env *head, char *key)
{
	t_env	*env;

	env = head;
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (env->value);
		env = env->next;
	}
	return ("");
}
void	free_env(t_env *env)
{
	t_env	*tmp;
	t_env	*next;

	tmp = env;
	while (tmp)
	{
		if (env->key)
			free(env->key);
		if (env->value)
			free(env->value);
		next = tmp->next;
		free(tmp);
		tmp = next;
	}
}
t_env	*create_env(char *key, char *value, int has_equal)
{
	t_env	*env;

	if (!ftt_calloc(1, sizeof(t_env), (void *)&env))
		exit(1);
	env->key = ft_strdup(key);
	env->has_equal = has_equal;
	env->next = 0;
	if (value)
		env->value = ft_strdup(value);
	else
		env->value = ft_strdup("");
	return (env);
}
void	update_env(t_env *head, char *key, char *value, int has_equal)
{
	t_env	*env;

	env = find_env(head, key);
	if (env == NULL)
		add_env_back(&head, key, value, has_equal);
	else
	{
		if (env->value)
		{
			free(env->value);
			if (value)
				env->value = ft_strdup(value);
			else
				env->value = 0;
		}
		else
		{
			if (value)
				env->value = ft_strdup(value);
		}
		env->has_equal = has_equal;
	}
}
void	add_env_back(t_env **head, char *key, char *value, int has_equal)
{
	t_env	*env;

	if (*head == NULL)
		*head = create_env(key, value, has_equal);
	else
	{
		env = *head;
		while (env->next)
			env = env->next;
		env->next = create_env(key, value, has_equal);
	}
}
void	parse_env(char **envp, t_state *state)
{
	int		i;
	int		j;
	char	*key;
	char	*value;

	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j])
		{
			if (envp[i][j] == '=')
			{
				key = ft_substr(envp[i], 0, j);
				value = ft_substr(envp[i], j + 1, ft_strlen(envp[i]) - j - 1);
				break ;
			}
			j++;
		}
		add_env_back(&(state->env_head), key, value, TRUE);
		free(key);
		free(value);
		i++;
	}
	update_env(state->env_head, "OLDPWD", "", FALSE);
}
void	change_dollar_sign(int i)
{
	int		len;
	char	*key;
	char	*temp;
	char	*value;
	char	*input;

	input = g_state.input2;
	len = check_key_len(&input[i + 1], TRUE);
	key = ft_substr(&input[i + 1], 0, len);
	if (!ft_strcmp(key, "?"))
	{
		temp = ft_itoa(g_state.ret);
		value = ft_strdup(temp);
		free(temp);
	}
	else
		value = ft_strdup(find_env_val(g_state.env_head, key));
	free(key);
	temp = changed_str(input, i, i + len, value);
	free(g_state.input2);
	g_state.input2 = ft_strdup(temp);
	free(temp);
}


/**************************************************************/
/*							Prompt							  */
/**************************************************************/

void	delete_save(t_save *save)
{
	if (save->prev != 0)
		save->prev->next = save->next;
	if (save->next != 0)
		save->next->prev = save->prev;
	free(save->input);
	free(save);
	save = 0;
}
void	reset_save(t_state *s)
{
	t_save	*tmp;

	if (s->save_head == 0)
		return ;
	while (s->save_head->prev)
		s->save_head = s->save_head->prev;
	tmp = s->save_head->next;
	if (s->save_head->flag == 0)
	{
		delete_save(s->save_head);
		s->save_head = tmp;
	}
	s->s_flag = 0;
}
t_save	*push_front_save(char *input, t_save *old_head, int flag)
{
	t_save	*new;

	new = 0;
	if (!ftt_calloc(1, sizeof(t_save), (void *)&new))
		exit(1);
	new->input = ft_strdup(input);
	new->prev = 0;
	new->next = old_head;
	new->flag = flag;
	if (old_head != 0)
		old_head->prev = new;
	return (new);
}
void	save_history(t_state *s)
{
	reset_save(s);
	if (s->input == 0)
		return ;
	if (s->save_head == 0)
	{
		s->save_head = push_front_save(s->input, 0, 1);
	}
	else
	{
		s->save_head = push_front_save(s->input, s->save_head, 1);
	}
}
char	*ft_strcjoin(char *str, char c)
{
	char	*ret;
	int		i;

	if (!str)
	{
		if (!ftt_calloc(2, sizeof(char), (void *)&ret))
			exit(1);
		ret[0] = c;
		ret[1] = '\0';
	}
	else
	{
		if (!ftt_calloc(ft_strlen(str) + 2, sizeof(char), (void *)&ret))
			exit(1);
		i = -1;
		while (str[++i])
			ret[i] = str[i];
		ret[i] = c;
		ret[i + 1] = '\0';
		free(str);
	}
	return (ret);
}
void	print_save_char(t_state *s, char c)
{
	s->input = ft_strcjoin(s->input, c);
	write(1, &c, 1);
}
void	press_down(t_state *s)
{
	if (s->save_head == 0 || (s->save_head->prev == 0
			&& s->save_head->flag == 0))
		return ;
	if (s->save_head->prev == 0)
		clear_prompt(s);
	else
	{
		s->save_head = s->save_head->prev;
		move_cursor(s);
		if (s->input)
			free(s->input);
		s->input = 0;
		write(1, s->save_head->input, ft_strlen(s->save_head->input));
		s->input = ft_strdup(s->save_head->input);
	}
}
void	press_up(t_state *s)
{
	if (!s->save_head)
		return ;
	if (s->input != 0 && s->s_flag == 0)
		s->save_head = push_front_save(s->input, s->save_head, 0);
	if ((s->save_head->next != 0 && s->s_flag != 0) || s->save_head->flag == 0)
		s->save_head = s->save_head->next;
	s->s_flag = 1;
	move_cursor(s);
	write(1, s->save_head->input, ft_strlen(s->save_head->input));
	if (s->input)
		free(s->input);
	s->input = ft_strdup(s->save_head->input);
}
void	put_backspace(t_state *s)
{
	int	col;
	int	row;

	set_cursor(&col, &row);
	if (!s->input || (s->start.row >= row && s->start.col >= col))
		return ;
	col--;
	if (col < 0)
	{
		row -= 1;
		col = s->max.col;
	}
	tputs(tgoto(s->t.cm, col, row), 1, ft_putchar);
	tputs(s->t.ce, 1, ft_putchar);
	s->input = delete_last_char(s->input);
}
void	handle_eof(t_state *s, char *input)
{
	if (!input)
	{
		printf("exit\n");
		tcsetattr(STDIN_FILENO, TCSANOW, &s->t.save);
		exit(0);
	}
}
void	handle_keycode(t_state *s, int keycode)
{
	if (keycode == 4)
		handle_eof(s, s->input);
	else if (keycode == 127)
		put_backspace(s);
	else if (keycode == 4283163)
		press_up(s);
	else if (keycode == 4348699)
		press_down(s);
	else
	{
		if (ft_isprint((char)keycode))
			print_save_char(s, (char)keycode);
	}
}
char	*delete_last_char(char *str)
{
	char	*tmp;
	int		len;
	int		i;

	if (str == 0)
		return (0);
	len = ft_strlen(str);
	if (!ftt_calloc(len, sizeof(char), (void *)&tmp))
		exit(1);
	i = 0;
	while (str[i + 1])
	{
		tmp[i] = str[i];
		i++;
	}
	tmp[len - 1] = '\0';
	free(str);
	if (ft_strlen(tmp) == 0)
	{
		free(tmp);
		tmp = 0;
	}
	str = tmp;
	return (str);
}
int		is_backslash(t_state *s)
{
	int		flag;
	int		i;
	char	*str;

	if (s->input == 0 || ft_strlen(s->input) == 0)
		return (0);
	str = s->input;
	flag = -1;
	i = 0;
	while (str[i])
		i++;
	i--;
	while (str[i] == '\\' && i >= 0)
	{
		flag *= -1;
		i--;
	}
	if (flag == 1)
	{
		s->input = delete_last_char(str);
		return (1);
	}
	else
		return (0);
}
int		get_nbr_len(int n)
{
	int	ret;

	ret = 0;
	while (n > 0)
	{
		n /= 10;
		ret++;
	}
	return (ret);
}
void	init_set_cursor(char buf[255], int *read_ret, int *i, int *flag)
{
	write(0, "\033[6n", 4);
	*read_ret = read(0, buf, 254);
	if (*read_ret < 0)
		*read_ret = 0;
	buf[*read_ret] = '\0';
	*i = 0;
	*flag = 0;
}
void	set_cursor(int *col, int *row)
{
	int		i;
	int		flag;
	char	buf[255];
	int		read_ret;

	init_set_cursor(buf, &read_ret, &i, &flag);
	while (buf[++i])
	{
		if ('0' <= buf[i] && buf[i] <= '9')
		{
			if (flag == 0)
				*row = ft_atoi(&buf[i]) - 1;
			else
			{
				*col = ft_atoi(&buf[i]) - 1;
				break ;
			}
			flag++;
			i += get_nbr_len(*row) - 1;
		}
	}
}
void	set_cursor_win(t_state *s)
{
	struct winsize	w;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	s->max.col = w.ws_col;
	s->max.row = w.ws_row;
	set_cursor(&s->start.col, &s->start.row);
}
void	move_cursor(t_state *s)
{
	t_keypos	pos;
	int			len;

	if (s->input != 0)
		len = ft_strlen(s->input);
	else
		len = 0;
	set_cursor(&pos.col, &pos.row);
	pos.col -= len;
	if (pos.col < 0)
	{
		pos.row--;
		pos.col = s->max.col + pos.col;
	}
	tputs(tgoto(s->t.cm, pos.col, pos.row), 1, ft_putchar);
	tputs(tgetstr("ce", NULL), 1, ft_putchar);
}
void	clear_prompt(t_state *s)
{
	move_cursor(s);
	if (s->input)
		free(s->input);
	s->input = 0;
	s->s_flag = 0;
}
int		term_loop(t_state *s)
{
	int	c;

	set_cursor_win(s);
	set_cursor(&s->cur.col, &s->cur.row);
	c = 0;
	while (read(0, &c, sizeof(c)) > 0)
	{
		if (c == '\n')
		{
			write(1, "\n", 1);
			if (is_backslash(s))
				return (1);
			else
				return (0);
		}
		else
			handle_keycode(s, c);
		c = 0;
	}
	return (0);
}
void	init_term(t_state *s)
{
	tcgetattr(STDIN_FILENO, &s->t.term);
	s->t.term.c_lflag &= ~ICANON;
	s->t.term.c_lflag &= ~ECHO;
	s->t.term.c_cc[VMIN] = 1;
	s->t.term.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &s->t.term);
	tgetent(NULL, "xterm");
	s->t.cm = tgetstr("cm", NULL);
	s->t.ce = tgetstr("ce", NULL);
}
void	prompt2(t_state *s)
{
	write(1, "bash", 4);
	init_term(s);
	while (1)
	{
		write(1, "> ", 2);
		if (term_loop(s) == 0)
		{
			reset_save(s);
			if (g_state.input)
				g_state.input2 = ft_strdup(g_state.input);
			break ;
		}
		else
			continue ;
	}
}


/**************************************************************/
/*							Main Stuff						  */
/**************************************************************/

void	init_state(t_state *state)
{
	ft_memset((void *)state, 0, sizeof(t_state));
	tcgetattr(0, &state->t.save);
}

int	main(int argc, char **argv, char **envp)
{
	set_history();

	init_state(&g_state);
	signal(SIGINT, (void *)handle_signal);
	signal(SIGQUIT, (void *)handle_signal);

	parse_env(envp, &g_state);

	while (1)
	{
		prompt2(&g_state);
		tcsetattr(0, TCSANOW, &g_state.t.save);

		g_state.input = ft_strdup("ola");
		save_history(&g_state);
	}

	return (0);

}
