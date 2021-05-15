/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcaps.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 18:38:30 by tisantos          #+#    #+#             */
/*   Updated: 2021/05/15 22:45:35 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <string.h>
# include <signal.h>
# include <errno.h>
# include <termios.h>
# include <termcap.h>
# include <fcntl.h>

int		ft_putint(int c);
void	prompt();
void	turn_on_canonical_mode();

/**************************************************************/
/*						Structs								  */
/**************************************************************/

typedef struct s_minishell
{
	int				islinux;

	char			*line; // Line you write on your stdin.

	char			**env;	 // Env.

	char			**history;	 // Saves the history of commands you typed.
	int				history_len; // How many commands you've typed.
	int				current_history;


	struct termios	old_term;
	struct termios	new_term;
	char			*buffer;

	char			*keys_on;
	char			*keys_off;
	char			*up_arrow;
	char			*down_arrow;
	char			*backspace;
	char			*del_line;
	char			*cursor_left;

	char			last_key_pressed;

} t_minishell;

t_minishell mini_sh;



/**************************************************************/
/*						   Utils							  */
/**************************************************************/

char	*ft_strcpy(char *dest, char *src)
{
	int i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
void	ft_bzero(void *s, size_t n)
{
	unsigned char	*str_ptr;
	size_t			i;

	str_ptr = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		str_ptr[i] = 0;
		i++;
	}
}
char	*ft_strdup(const char *s)
{
	char	*dest;
	int		len;
	int		i;

	len = 0;
	i = 0;
	while (s[len] != '\0')
		len++;
	dest = malloc(sizeof(*dest) * len + 1);
	if (dest == NULL)
		return (NULL);
	while (i < len)
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
char	*ft_strchr(const char *str, int c)
{
	int		i;
	char	*new_str;

	i = 0;
	new_str = (char *)str;
	while (str[i] != c)
	{
		if (str[i] == '\0')
			return (NULL);
		i++;
		new_str++;
	}
	return (new_str);
}
int		ft_putint(int c)
{
	return (write(1, &c, 1));
}
int		ft_strcmp(char *s1, char *s2)
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
size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}
int		ft_strncmp(const char *str1, const char *str2, size_t n)
{
	int		i;
	size_t	len;

	i = 0;
	len = n;
	if (n == 0)
		return (0);
	while (str1[i] != '\0' && str2[i] != '\0' && len - 1 > 0)
	{
		if (str1[i] == str2[i])
		{
			i++;
			len--;
		}
		else
			break ;
	}
	return ((unsigned char)str1[i] - (unsigned char)str2[i]);
}
void	*ft_calloc(size_t nitems, size_t size)
{
	char			*arra;
	size_t			i;
	unsigned int	total;

	total = nitems * size;
	arra = malloc(nitems * size);
	if (arra == NULL)
		return (NULL);
	i = 0;
	while (total > 0)
	{
		arra[i] = 0;
		total--;
		i++;
	}
	return ((void *)arra);
}
void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i] != NULL)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void ft_exit()
{
	if (mini_sh.keys_off)
		tputs(mini_sh.keys_off, 1, ft_putint);
	if (mini_sh.islinux == 0)
		free(mini_sh.backspace);
	turn_on_canonical_mode();
}


/**************************************************************/
/*						   History							  */
/**************************************************************/

int		has_history()
{
	if (mini_sh.history_len == 0)
		return(0);
	else
		return(1);
}
void	parse_input_history(char *buf, int *i)
{
	char	*input;
	int		only_up_arrow;

	only_up_arrow = 0;

	if (mini_sh.current_history == mini_sh.history_len)
		only_up_arrow = 1;
	if (has_history() == 0)
	{
		ft_bzero(&buf[*i], BUFSIZ - *i);
		return ;
	}
	if (ft_strcmp(mini_sh.up_arrow, &buf[*i]) == 0)
	{
		if (mini_sh.current_history > 0)
			mini_sh.current_history--;
		input = mini_sh.history[mini_sh.current_history];
		tputs(mini_sh.cursor_left, 1, ft_putint);
		tputs(mini_sh.del_line, 1, ft_putint);
		prompt();
		ft_bzero(buf, BUFSIZ);
		ft_strcpy(buf, input);
		*i = write(STDOUT_FILENO, buf, ft_strlen(buf));
	}
	else if (ft_strcmp(mini_sh.down_arrow, &buf[*i]) == 0 && only_up_arrow == 0)
	{
		if (mini_sh.current_history < mini_sh.history_len - 1)
			mini_sh.current_history++;
		input = mini_sh.history[mini_sh.current_history];
		tputs(mini_sh.cursor_left, 1, ft_putint);
		tputs(mini_sh.del_line, 1, ft_putint);
		prompt();
		ft_bzero(buf, BUFSIZ);
		ft_strcpy(buf, input);
		*i = write(STDOUT_FILENO, buf, ft_strlen(buf));
	}
}
void	save_history()
{
	if (mini_sh.history_len > 0 &&
		ft_strcmp(mini_sh.history[mini_sh.history_len - 1], mini_sh.line) == 0)
		return;

	mini_sh.history[mini_sh.history_len] = ft_strdup(mini_sh.line);
	mini_sh.history_len++;
	mini_sh.history[mini_sh.history_len] = NULL;
}
void	set_history()
{
	mini_sh.history = malloc(sizeof(char *) + 100);
	if (mini_sh.history == NULL)
		return;
	mini_sh.history_len = 0;
	mini_sh.history[0] = NULL;
}


/**************************************************************/
/*						Termcaps Loop						  */
/**************************************************************/

void	turn_off_canonical_mode()
{
	mini_sh.new_term = mini_sh.old_term;
	mini_sh.new_term.c_lflag &= ~ICANON;
	mini_sh.new_term.c_lflag &= ~ECHO;
	mini_sh.new_term.c_lflag &= ~ISIG;
	mini_sh.new_term.c_cc[VMIN] = 1;
	mini_sh.new_term.c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &mini_sh.new_term) == -1)
		ft_exit(EXIT_FAILURE);
}
void	turn_on_canonical_mode()
{
	if (tcsetattr(STDIN_FILENO, TCSANOW, &mini_sh.old_term) == -1)
		ft_exit();
}

/**************************************************************/
/*						Termcaps Init						  */
/**************************************************************/

int		set_capabilities()
{
	mini_sh.keys_on = tgetstr("ks", &mini_sh.buffer);
	if (mini_sh.keys_on)
		tputs(mini_sh.keys_on, 1, ft_putint);
	mini_sh.keys_off = tgetstr("ke", &mini_sh.buffer);
	mini_sh.up_arrow = tgetstr("ku", &mini_sh.buffer);
	mini_sh.down_arrow = tgetstr("kd", &mini_sh.buffer);
	if (mini_sh.islinux == 1)
		mini_sh.backspace = tgetstr("kb", &mini_sh.buffer);
	else
		mini_sh.backspace = ft_strdup("\x7f");
	mini_sh.del_line = tgetstr("dl", &mini_sh.buffer);
	mini_sh.cursor_left = tgetstr("cr", &mini_sh.buffer);
	if ((!mini_sh.keys_on) || (!mini_sh.up_arrow) || (!mini_sh.down_arrow)
		|| (!mini_sh.backspace) || (!mini_sh.del_line) || (!mini_sh.keys_off)
		|| (!mini_sh.cursor_left))
		return (0);
	else
		return (1);
}
char	*get_termtype()
{
	int i = 0;
	char *temp;

	temp = NULL;

	while (mini_sh.env[i] != NULL)
	{
		if (ft_strncmp(mini_sh.env[i], "TERM=", 5) == 0)
		{
			mini_sh.env[i] += 5;
			temp = mini_sh.env[i];
			break;
		}
		i++;
	}
	return (temp);
}
void	init_termcaps()
{
	char *term_type;
	int ret;

	if (isatty(STDIN_FILENO) != 1)
		exit(EXIT_FAILURE);

	if (tcgetattr(STDIN_FILENO, &mini_sh.old_term) == -1)
		exit(EXIT_FAILURE);

	if (mini_sh.islinux == 1)
	{
		mini_sh.buffer = ft_calloc(2048, 1);
		if (mini_sh.buffer == NULL)
			exit(EXIT_FAILURE);
	}

	term_type = get_termtype();
	if (term_type == NULL)
		exit(EXIT_FAILURE);

	ret = tgetent(mini_sh.buffer, term_type);
	if (ret <= 0)
		exit(EXIT_FAILURE);
	else if (set_capabilities() == 0)
		exit(EXIT_FAILURE);

}


/**************************************************************/
/*						   Input							  */
/**************************************************************/

void	delete_single_char(char *buf, int *i)
{
	ft_bzero(&buf[*i - 1], BUFSIZ - *i);
	tputs(mini_sh.cursor_left, 1, ft_putint);
	tputs(mini_sh.del_line, 1, ft_putint);
	prompt();
	*i = write(STDOUT_FILENO, buf, ft_strlen(buf));
}
int		is_up_down_arrow(char *buf)
{
	if (ft_strcmp(mini_sh.up_arrow, buf) == 0)
		return (1);
	else if (ft_strcmp(mini_sh.down_arrow, buf) == 0)
		return (1);
	return (0);
}
void	get_input()
{
	char buf[BUFSIZ];
	int i;
	int nb_char_read;

	i = 0;

	ft_bzero(buf, BUFSIZ);
	mini_sh.current_history = mini_sh.history_len;

	while(!ft_strchr(buf, '\n'))
	{
		nb_char_read = read(STDIN_FILENO, &buf[i], BUFSIZ - i);
		if (is_up_down_arrow(&buf[i]))
			parse_input_history(buf, &i);
		else if (ft_strcmp(&buf[i], mini_sh.backspace) == 0)
			delete_single_char(buf, &i);
		else if (nb_char_read > 1)
			ft_bzero(&buf[i], BUFSIZ - i);
		else
			i += write(STDOUT_FILENO, &buf[i], 1);
	}

	buf[i - 1] = '\0';
	mini_sh.line = ft_strdup(buf);
	if (mini_sh.line == NULL)
		ft_exit();
}
void	prompt()
{
	write(1, "\x1b[90mminishell\x1b[91m$ ", 22);
}

/**************************************************************/
/*						Main Stuff							  */
/**************************************************************/

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	mini_sh.islinux = 1;
	mini_sh.env = envp;
	set_history();

	init_termcaps();

	while (1)
	{
		turn_off_canonical_mode();
		mini_sh.line = NULL;

		prompt();
		get_input();
		save_history();

		turn_on_canonical_mode();

		//execute();

		//**********************************************//
		if (strcmp(mini_sh.line, "history") == 0)
		{
			for (size_t a = 0; mini_sh.history[a] != NULL; a++)
				printf("%li  %s\n", a, mini_sh.history[a]);
		}
		else if (strcmp(mini_sh.line, "exit") == 0)
		{
			free(mini_sh.line);
			free_array(mini_sh.history);
			break;
		}
		free(mini_sh.line);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * /
 *
 * http://www.physics.udel.edu/~watson/scen103/ascii.html
 *
 *											Intro:
 *
 *		This program doesn't follow the norminette and you'll have to change some stuff
 *	accordingly to fit your Minishell.
 *		Our History for instance is a (char **). Some use linked lists. If you use it,
 *	you have to make adjustments to fit your code.
 *
 *		This code is based on Dimitri's explanation on his throughtly research from
 *	other people's Minishell. Go buy him a beer!
 *	- https://github.com/DimitriDaSilva
 *	- dda-silv on intra.
 *
 *		Assuming you are this far, you know how Minishell works so I won't explain
 *	the loop, parsing, execute... But I'll try to keep it easy to follow.
 *
 *
 * 									How This Program Works:
 *
 *	0.	First things first. You need a global variable. We have one called mini_sh.
 *		It has the needed variables for this program.
 *
 *	1.	You need to set manually the value of islinux. If you are on Linux, set it to 1,
 *		if you are on Mac set it to 0. Save env to the global variable and initialize
 *		the history with set_history().
 *
 *	2.	Initialize termcaps with init_termcaps().
 *
 *		isatty(fd):
 *			It checks if we're running a valid descriptor (Standard Input)
 *			with a terminal. If we're not, we'll exit the program.
 *
 *		tcgetattr(fd, struct termios):
 *			Function shall get the parameters associated with the terminal referred to
 *			by fildes and store them in the termios structure referenced by termios_p.
 *			You need to start this function before settings the parameters.
 *
 *		On Linux you have to allocate memory on the buffer. On Mac you don't need to.
 *
 *		You need to get the TERM= from env. But you only need the part after = .
 *		xterm-256color.
 *
 * 		Initialize the termcap with tgetent and then set capabilites.
 *
 *		The tgetstr routine returns the string entry for id, or zero if it is not
 *		available. Use tputs to output the returned string.
 *
 * 		Here you can see why we used the things we did.
 * 		https://www.gnu.org/software/termutils/manual/termcap-1.3/html_mono/termcap.html
 *
 *	3.		Canonical Input - When you write on your terminal and you have to press enter
 *		to enter the command.
 *			Non Canonical Input - When you type on your terminal, and each letter you type
 *		it enters immediately.
 *			Echo - Is when you're typing and it's invisible. Like when you do sudo and type
 *		your password.
 *
 *		On the start of the loop we make it Non Canonical Mode.
 *
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

