#include "../../shell.h"

int	only_export(void)
{
	int	i;

	i = 0;
	while (mini_sh.env[i])
	{
		printf("declare -x %s\n", mini_sh.env[i++]);
	}
	return (0);
}

int	ft_export(char **content)
{
	t_built_ex_var	va;

	va.i = 0;
	if (!content[1])
	{
		only_export();
		return (0);
	}
	if (ft_isdigit((int)content[1][0]))
	{
		printf("bash: export: `%s\': not a valid identifier\n", content[1]);
		return (-1);
	}
	if (!ft_strchr(content[1], '='))
		return (-1);
	while (content[1][va.i] && content[1][va.i] != '=')
		va.i++;
	va.set = ft_substr(content[1], 0, va.i);
	va.elem = env_isex_elem(va.set);
	if (ft_strlen(va.elem))
		env_rm_elem(va.set);
	free(va.set);
	mini_sh.env = add_str_to_arrarr(mini_sh.env, content[1]);
	return (1);
}
