#include "../../shell.h"
void	ft_putnstr(char *str, int n)
{
	int		i;

	i = -1;
	if (n < 0)
	{
		while (str[++i] && i < (int)ft_strlen(str) + n)
			write(1,&str[i],1);
	}
	else
	{
		while (str[++i] && i < n)
			write(1,&str[i],1);
	}
}

static void		echo_out(char **str, int pos)
{
	int		starts_with;
	int		ends_with;
	int		str_len;

	starts_with = (str[pos][0] == '"');
	str_len = (int)ft_strlen(str[pos]);
	ends_with = (str[pos][str_len - 1] == '"');
	if (ends_with && starts_with)
		ft_putnstr(str[pos] + 1, -1);
	else if (ends_with)
		ft_putnstr(str[pos], -1);
	else if (starts_with)
		ft_putstr_fd(str[pos] + 1,1);
	else
		ft_putstr_fd(str[pos],1);
	if (str[pos + 1])
		write(1," ",1);
}


int				ft_echo()
{
	int		i;
	int		n_flag;

	n_flag = 0;
	if (!mini_sh.args[1])
	{
		write(1, "\n", 1);
		return (1);
	}
	else if (!ft_strcmp(mini_sh.args[1] ,"-n"))
		n_flag = 1;
	i = 0;
	if (n_flag)
		++i;
	while (mini_sh.args[++i])
	{
		echo_out(mini_sh.args, i);
		if (!mini_sh.args[i + 1] && !n_flag)
			write(1,"\n",1);
	}
	return (1);
}
