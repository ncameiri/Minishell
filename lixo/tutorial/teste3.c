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

# define READLINE_BUFSIZE 100
# define CMDHIST_BUF 20
# define BUILTINS "exit,pwd,export,env,echo,cd,unset"
# define DQUOTE_ESC_CHARS "\"$\\"
# define DEL 127
# define SUB 26
# define NUL ""
# define SYN_ERROR "micli: syntax error near unexpected token"
# define BUF_OVERFLOW "\nBAD BUNNY! TOO MANY CHARACTERS IN LINE BUFFER!"

/**************************************************************/
/*						   Global Stuff						  */
/**************************************************************/

char **history;
int history_location;

struct termios	orig_term;

typedef struct s_cmdhist
{
	short			**hist_stack;
	short			*bufsize;
	short			*char_total;
	size_t			recalc_bufsize;
	size_t			cmdhist_buf;
	size_t			ptrs_in_hist;
	size_t			index;
	char			scroll;
}				t_cmdhist;

typedef struct s_insertcaps
{
	char	*insert_mode;
	char	*exit_insert;
	char	*insert_char;
	char	*insert_pad;
	char	*direct_insert;
	int		mov_while_ins_flag;
	int		spc_not_clr_flag;

}				t_insertcaps;

typedef struct s_deletecaps
{
	char	*delete_char;
	char	*delete_mode;
	char	*exit_delete;
}				t_deletecaps;

typedef struct s_termcaps
{
	char			termbuf[2048];
	char			capbuf[2048];
	t_insertcaps	inscaps;
	t_deletecaps	delcaps;
	char			*cur_up;
	char			*cur_left;
	char			*cur_right;
	char			*carriage_ret;
	char			*del_line;
	char			*arrow_up;
	char			*arrow_left;
	char			*arrow_right;
	char			*arrow_down;
	short			cursor_keys_seq;
	short			keypad_seq_up;
	short			keypad_seq_down;
	char			init_result;
}				t_termcaps;

typedef struct s_micli
{
	t_cmdhist		cmdhist;
	t_termcaps		termcaps;
	struct winsize	winsize;
	struct termios	orig_term;
	struct termios	raw_term;
	int				home_row;
	int				syserror;
	int				cmd_result;
	char			*cmd_result_str;
	char			**envp;
	char			*buffer;
	short			*active_line;
	unsigned char	quote_flag : 1;
	unsigned char	pipe_flag : 2;
}				t_micli;


/**************************************************************/
/*							 UTILS 							  */
/**************************************************************/

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

/* Finds the var in env. Examples TERM= , HOME= , etc...*/

char	*find_var(const char *name, size_t name_len, char **envp)
{
	char	**ptr;

	ptr = NULL;
	if (envp)
	{
		ptr = envp;
		while (*ptr && (ft_strncmp(name, *ptr, name_len) \
		 || (*(*ptr + name_len) != '=' && *(*ptr + name_len) != '\0')))
			ptr++;
	}
	return (*ptr);
}

/* Puts the envp on the micli.envp */

char	*clean_ft_strdup(char const *str, t_micli *micli)
{
	char	*ptr;

	ptr = ft_strdup(str);
	return (ptr);
}

void	*clean_calloc(size_t count, size_t size, t_micli *micli)
{
	void	*ptr;

	ptr = ft_calloc(count, size);
	return (ptr);
}

size_t	ft_countarr(char **envp)
{
	size_t	i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}

char	**ft_envdup(char **envp, t_micli *micli)
{
	size_t	envp_size;
	size_t	i;
	size_t	j;
	char	**new_envp;

	envp_size = ft_countarr(envp);
	i = envp_size;
	while (--i)
	{
		if (!(ft_strchr(envp[i], '=')))
			envp_size--;
	}
	new_envp = clean_calloc(envp_size + 1, sizeof(char *), micli);
	i = 0;
	j = 0;
	while (i < envp_size)
	{
		if (ft_strchr(envp[j], '='))
		{
			new_envp[i] = clean_ft_strdup(envp[j], micli);
			i++;
		}
		j++;
	}
	return (new_envp);
}


/**************************************************************/
/*					  Read Line from Loop					  */
/**************************************************************/

static void	handle_esc_seq_phase_1(short chr, char *esc_seq, t_micli *micli)
{
	if ((short)chr == '[' || (short)chr == micli->termcaps.arrow_up[1] \
	 || (short)chr == micli->termcaps.arrow_down[1])
		*esc_seq = 2;
	else
		*esc_seq = 0;
}

size_t	del_from_buf(short *chr, size_t num_chars)
{
	size_t	deleted_chars;

	deleted_chars = num_chars;
	while (num_chars--)
		*chr-- = 0;
	return (deleted_chars);
}


static int	is_special(short chr, char esc_seq, t_micli *micli)
{
	if (esc_seq || ((ft_iscntrl(chr) && chr != '\n') || chr == 23323 \
	 || chr == micli->termcaps.keypad_seq_up \
	 || chr == micli->termcaps.keypad_seq_down))
		return (1);
	return (0);
}

char	do_not_echo(short *buf, short *char_total, char *scroll_flag, \
t_micli *micli)
{
	static char		esc_seq = 0;
	short			chr;

	chr = buf[*char_total - 1];
	if (is_special(chr, esc_seq, micli))
	{
		*char_total -= del_from_buf(&buf[*char_total - 1], 1);
		if (!esc_seq)
			handle_unescaped_special(chr, buf, &esc_seq, micli);
		else if (esc_seq == 1)
			handle_esc_seq_phase_1(chr, &esc_seq, micli);
		else if (esc_seq == 2)
		{
			esc_seq = 0;
			if (chr == micli->termcaps.arrow_up[2])
				*scroll_flag = 1;
			else if (chr == micli->termcaps.arrow_down[2])
				*scroll_flag = -1;
		}
		return (1);
	}
	return (0);
}


void	do_scroll(t_micli *micli, t_cmdhist *cmdhist, short **hist_stack)
{
	if (cmdhist->scroll == 1)
		cmdhist->index--;
	if (cmdhist->scroll == -1)
		cmdhist->index++;
	remove_prompt_line(micli, *cmdhist->char_total);
	cmdhist->char_total = &hist_stack[cmdhist->index][0];
	cmdhist->bufsize = &hist_stack[cmdhist->index][1];
	write(STDOUT_FILENO, "🚀 ", 5);
	write(STDOUT_FILENO, &hist_stack[cmdhist->index][2], \
	*cmdhist->char_total * sizeof(short));
}

int	must_scroll(t_cmdhist *cmdhist)
{
	if ((cmdhist->scroll == 1 && cmdhist->index > 0) \
	 || (cmdhist->scroll == -1 && cmdhist->index < cmdhist->ptrs_in_hist - 1))
		return (1);
	return (0);
}

void	scratch_log_init(t_micli *micli, t_cmdhist *cmdhist, short **hist_stack)
{
	cmdhist->index = cmdhist->ptrs_in_hist - 1;
	hist_stack[cmdhist->index] = \
	clean_calloc(READLINE_BUFSIZE + 3, sizeof(short), micli);
	cmdhist->char_total = &hist_stack[cmdhist->index][0];
	cmdhist->bufsize = &hist_stack[cmdhist->index][1];
	*cmdhist->bufsize = READLINE_BUFSIZE;
}



short	*micli_readline(t_micli *micli, t_cmdhist *cmdhist, short **hist_stack)
{
	ssize_t	size;

	scratch_log_init(micli, cmdhist, hist_stack);
	while (1)
	{
		if (must_scroll(cmdhist))
			do_scroll(micli, cmdhist, hist_stack);
		cmdhist->scroll = 0;
		size = read(STDIN_FILENO, &hist_stack[cmdhist->index][*cmdhist->char_total + 2], 2);
		if (hist_stack[cmdhist->index][*cmdhist->char_total + 2] == 4 || !size)
		{
			write(1, "exit\n", 5);
		}
		*cmdhist->char_total += 1;
		if (!do_not_echo(&hist_stack[cmdhist->index][2], \
		cmdhist->char_total, &cmdhist->scroll, micli))
		{
			insert_char(&micli->termcaps, \
			hist_stack[cmdhist->index][*cmdhist->char_total + 1]);
			if (hist_stack[cmdhist->index][*cmdhist->char_total + 1] == '\n')
				return (send_to_parser(cmdhist, hist_stack));
		}
		if (*cmdhist->char_total == *cmdhist->bufsize)
			reallocate_readline_buffer(micli, cmdhist, hist_stack);
	}
}

/**************************************************************/
/*						Stuff from Loop						  */
/**************************************************************/

void	enable_raw_mode(struct termios *raw_term, struct termios *orig_term)
{
	ft_bzero(raw_term, sizeof(struct termios));
	*raw_term = *orig_term;
	raw_term->c_iflag |= (IUTF8);
	raw_term->c_iflag &= ~(BRKINT | INPCK | ISTRIP | IXON);
	raw_term->c_cflag |= (CS8);
	raw_term->c_lflag &= ~(ECHO | ICANON);
	tcsetattr(STDIN_FILENO, TCSADRAIN, raw_term);
	write(STDOUT_FILENO, "\e[?1h\e=", 7);
}


/**************************************************************/
/*						 Termcaps Init						  */
/**************************************************************/

// https://www.gnu.org/software/termutils/manual/termcap-1.3/html_chapter/termcap_4.html#SEC32

void	termcaps_init_fail(t_micli *micli, int failure, char *termtype)
{
	micli->syserror = errno;
	if (!termtype || !*termtype)
		printf("💥 TERM is unset. Set a terminal type: export TERM=<type>\n");
	else if (failure == 0)
		printf("💥 Terminal type '%s' not found in database.\n", termtype);
	else if (failure == -1)
		printf("💥 Could not access the termcap database.\n");
	else if (failure == -2)
		printf("💥 Your terminal lacks required basic capabilities...\n");
	else
		return ;
}

void	derive_esc_seq(t_termcaps *tcaps)
{
	tcaps->keypad_seq_up = (short)(((unsigned char)tcaps->arrow_up[0]) \
	 | (((unsigned char)tcaps->arrow_up[1]) << 8));
	tcaps->keypad_seq_down = (short)(((unsigned char)tcaps->arrow_down[0]) \
	 | (((unsigned char)tcaps->arrow_down[1]) << 8));
}

int	misc_mandatory_caps(t_termcaps *tcaps, char **capbuf) // Here we set all the mandatory
{														 // capabilities.
	tcaps->cur_up = tgetstr("up", capbuf);
	tcaps->cur_left = tgetstr("le", capbuf);
	tcaps->cur_right = tgetstr("nd", capbuf);
	tcaps->carriage_ret = tgetstr("cr", capbuf);
	tcaps->del_line = tgetstr("dl", capbuf);
	tcaps->arrow_up = tgetstr("ku", capbuf);
	tcaps->arrow_left = tgetstr("kl", capbuf);
	tcaps->arrow_right = tgetstr("kr", capbuf);
	tcaps->arrow_down = tgetstr("kd", capbuf);
	derive_esc_seq(tcaps);
	if (!tcaps->cur_up || !tcaps->cur_left || !tcaps->cur_right \
	 || !tcaps->carriage_ret || !tcaps->del_line || !tcaps->arrow_up \
	 || !tcaps->arrow_down || ft_strlen(tcaps->arrow_up) != 3 \
	 || ft_strlen(tcaps->arrow_down) != 3)
		return (0);
	return (1);
}

int	character_delete_caps(t_termcaps *tcaps, char **capbuf) // Here we set all the available
{														   // of insertion capabilities.
	tcaps->delcaps.delete_char = tgetstr("dc", capbuf);	  // Even if not being used.
	if (!tcaps->delcaps.delete_char)
		return (0);
	tcaps->delcaps.delete_mode = tgetstr("dm", capbuf);
	tcaps->delcaps.exit_delete = tgetstr("ed", capbuf);
	return (1);
}

int	character_insertion_caps(t_termcaps *tcaps, char **capbuf) // Here we set all the available
{														      // of insertion capabilities.
	tcaps->inscaps.insert_mode = tgetstr("im", capbuf);		 // Even if not being used.
	if (!tcaps->inscaps.insert_mode)
		tcaps->inscaps.insert_char = tgetstr("ic", capbuf);
	else
		tcaps->inscaps.exit_insert = tgetstr("ei", capbuf);
	tcaps->inscaps.insert_pad = tgetstr("ip", capbuf);
	tcaps->inscaps.mov_while_ins_flag = tgetflag("mi");
	tcaps->inscaps.spc_not_clr_flag = tgetflag("in");
	if (tcaps->inscaps.insert_mode || tcaps->inscaps.insert_char \
	 || tcaps->inscaps.insert_pad)
		return (1);
	return (0);
}

void	termcaps_init(t_micli *micli, t_termcaps *tcaps)
{
	char	*termtype;
	char	*capbuf;

	capbuf = &tcaps->capbuf[0];
	termtype = find_var("TERM", 4, micli->envp);	// Sets the Termtype to = "TERM=xterm-256color"
	if (termtype)
	{
		termtype += 5;
		if (*termtype)
			tcaps->init_result = tgetent(tcaps->termbuf, termtype); // Puts tgetent on init_result
	}
	if (tcaps->init_result > 0) // if tgentent did not give an error.
	{
		if (!character_insertion_caps(tcaps, &capbuf) \
		|| !character_delete_caps(tcaps, &capbuf) \
		|| !misc_mandatory_caps(tcaps, &capbuf))
		{
			tcaps->init_result = -2;
		}
	}
	termcaps_init_fail(micli, tcaps->init_result, termtype); // If it fails
}


/**************************************************************/
/*							 Main 							  */
/**************************************************************/

char	micli_loop(t_micli *micli)
{
	while (1)
	{
		enable_raw_mode(&micli->raw_term, &micli->orig_term);

		write(STDOUT_FILENO, "minicheila> ", 13);

		micli->active_line = micli_readline(micli, &micli->cmdhist, \
		micli->cmdhist.hist_stack);

	//	micli->active_line = clean_ft_memdup(micli->active_line, \
	//	(micli->cmdhist.recalc_bufsize + 3) * sizeof(short), micli);

	//	push_to_hist_stack(micli, micli->active_line, &micli->cmdhist);

	//	if (micli->active_line[2] == 0)
	//		micli->buffer = clean_ft_strdup("\0", micli);
	//	else
	//		micli->buffer = ft_short_to_strdup(&micli->active_line[2]);

	//	micli->active_line = ft_del(micli->active_line);

	//	tcsetattr(STDIN_FILENO, TCSAFLUSH, &micli->orig_term);

	//	process_raw_line(micli->buffer, micli);

	//	micli->buffer = ft_del(micli->buffer);

	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_micli	micli;

	set_history();
	micli.envp = ft_envdup(envp, &micli);

	tcgetattr(STDIN_FILENO, &orig_term); // Gets the attr.
	termcaps_init(&micli, &micli.termcaps);

	micli_loop(&micli);
	return (0);

}
