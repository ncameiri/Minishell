#include "../../shell.h"

int	ft_cd()
{
	char *path;
	if(chdir(mini_sh.args[1]) == -1)
	{
		ft_putstr_fd("cd: not a directory",1);
		ft_putstr_fd("\n",1);
	}
}
