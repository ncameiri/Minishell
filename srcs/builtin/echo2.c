#include "../../shell.h"

void	ft_putnstr2(t_splvariab	*varia, char *str, int n, int i)
{
	while (str[++i] && i < (int)ft_strlen(str) + n)
	{
		if (varia->its_clos)
			varia->its_clos = its_open_quo(varia, str[i + 1]);
		else
			varia->its_clos = its_open_quo(varia, str[i]);
		if (!ft_strchr(OPEN_QUOTE_EC, str[i]) || !varia->its_clos)
			write(1, &str[i], 1);
	}
}
