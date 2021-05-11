#include "../../shell.h"

char	*env_isex_elem (char *set)
{
	int		i;
	int		k;
	char	*set_equal;

	set_equal = ft_strjoin(set, "=");
	i = 0;
	k = ft_strlen(set_equal);
	while (mini_sh.env[i] != NULL)
	{
		if (!strncmp(set_equal, mini_sh.env[i], k))
		{
			free(set_equal);
			return (mini_sh.env[i] + k);
		}
		i++;
	}
	ft_strlen(set_equal);
	free(set_equal);
	return ("");
}

int	env_var_update(char *set_tbc, char *new_ct, int is_env)
{
	char	*elem;
	char	*set;

	env_rm_elem(set_tbc);
	set = ft_strjoin(set_tbc, "=");
	if (is_env)
		elem = ft_strjoin(set, env_isex_elem(new_ct));
	else
		elem = ft_strjoin(set, new_ct);
	mini_sh.env = add_str_to_arrarr(mini_sh.env, elem);
	free(set);
	free(elem);
	return (0);
}
