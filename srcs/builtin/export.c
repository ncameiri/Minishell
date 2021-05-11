#include "../../shell.h"

int only_export()
{
	int	i;

	i = 0;
	while(mini_sh.env[i])
	{
		printf("declare -x %s\n",mini_sh.env[i++]);
	}
	return(0);
}

int ft_export(char **content)
{
	char *elem;
	char *set;
	int i;

	i = 0;
	if (!content[1])
	{
		only_export();
		return(0);
	}
	if (ft_isdigit((int)content[1][0]))
	{
		printf("bash: export: `%s\': not a valid identifier\n",content[1]);
		return(-1);
	}
	if (!ft_strchr(content[1], '='))
		return -1;
	while (content[1][i] && content[1][i] != '=')
		i++;
	set = ft_substr(content[1], 0, i);
	elem = env_isex_elem(set);
	if (ft_strlen(elem))
	   env_rm_elem(set);
	free(set);
	mini_sh.env = add_str_to_arrarr(mini_sh.env, content[1]);
	return (1);
}
