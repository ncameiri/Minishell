#include "../../shell.h"

int	change_dir(char *path)
{
	char	*buf;
	int		ret;

	if (!ft_strncmp(path, "~", 1))
		buf = ft_strdup(env_isex_elem("HOME"));
	else if (!ft_strncmp(path, "-", 1))
		buf = ft_strdup(env_isex_elem("OLDPWD"));
	else
		buf = ft_strdup(path);
	if (!buf)
		return -1;
	ret = chdir(buf);
	free(buf);
	return (ret);
}

int	ft_cd()
{
	char	pwd[1001];

	
	if (!ft_strcmp(mini_sh.ls_start->content[1],"~"))
		change_dir("~");
	else if (change_dir(mini_sh.ls_start->content[1]) == -1)
	{
		printf("bash: cd: %s: No such file or directory\n", mini_sh.ls_start->content[1]);
		return -1;
	}
		getcwd(pwd, 1000);
		env_var_update("OLDPWD","PWD",1);
		env_var_update("PWD", pwd, 0);
	return (0);
}
