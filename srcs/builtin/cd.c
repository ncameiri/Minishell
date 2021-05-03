#include "../../shell.h"

int	ft_cd()
{
	char *path;
	t_linklis *ptr;
	int i;

	i = 1;
	ptr = mini_sh.ls_start;
	if(ptr->content[i] && !ft_strcmp(ptr->content[i],"-n"))
		i++;
	
	if (chdir(ptr->content[i]) == -1)
	{
		ft_putstr_fd("cd: not a directory",1);
		ft_putstr_fd("\n",1);
	}
}
